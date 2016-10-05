#include "FluxModel.h"
#include <math.h>
#include <string.h>

static const double deg = M_PI/180.;

static double _cutoff_rigidity_Stoermer(double theta, double phi, double altitude, double latitude)
{
    /*
        Magnetic cutoff rigidity, in GeV, according to Stoermer (D.F.. Smart and M.A. Shea, Advances
        in Space Research, 36 (2005) 2012-2020).
    */

    const double r  = 1.+altitude/6370e3;
    const double c  = cos(latitude*deg);
    const double se = sin(phi*deg);
    const double sz = sin(theta*deg);
    
    const double c3 = c*c*c;
    const double c4 = c3*c;
    
    double d = 1.0+sqrt(1.0-se*sz*c3);
    d *= d;
    
    return 14.5*c4/r*r*4.0/d;
}

static double _Chirkin_cos_thetastar(double x)
{
    /* 
        Cos(theta*) correction, x = cos(theta).
    */
    	
    const double p_cosstar[] = {0.102573, -0.068287, 0.958633, 0.0407253, 0.817285};
    const double p_cosstar02 = p_cosstar[0]*p_cosstar[0];
    
    return sqrt(
        (x*x+p_cosstar02+p_cosstar[1]*pow(x, p_cosstar[2])+p_cosstar[3]*pow(x, p_cosstar[4]))/(1.+p_cosstar02+p_cosstar[1]+p_cosstar[3])
    );
}

static double _Chirkin_mass_overburden(double x)
{
    /*
         Average mass overburden as function of x=cos(theta), in g/cm^2.
    */
    
    const double p_xtot[] = {-0.017326, 0.114236, 1.15043, 0.0200854, 1.16714};
    
    return 1e2/(p_xtot[0]+p_xtot[1]*pow(x, p_xtot[2])+p_xtot[3]*pow((1.-x*x), p_xtot[4]));
}

static double _Chirkin_path_length(double x)
{
    /*
        Average path length as function of x=cos(theta), in m.
    */
 
    const double p_length[] = {1.3144, 50.2813, 1.33545, 0.252313, 41.0344};
 
    return 1e6/(p_length[0]+p_length[1]*pow(x, p_length[2])+p_length[3]*pow((1.-x*x), p_length[4]));
}

static double _differential_flux_Chirkin(double momentum, double theta, double phi, double altitude, double* p_latitude)
{
    /*
        Differential flux for the given momentum, according to D. Chirkin (hep-ph/0407078). A muon decay 
	time correction for altitude was added. If a latitude was provided we also apply a raw cutoff on 
	the magnetic rigidity.
    */
    
    const double A0     = 0.14*0.701; /* normalization: GeV^-1 cm^-2 s^-1 sr^-1 */
    const double gamma  = 2.715;      /* spectral index                         */
    const double ctau   = 658.65;     /* [m]                                    */
    const double a      = 2.62e-3;    /* [GeV/(g/cm2)]                          */
    const double b      = 3.05e-6;    /* [1/(g/cm2)]                            */
    const double astar  = 0.487e-3;   /* [GeV/(g/cm2)^0.5]                      */
    const double bstar  = 8.766e-6;   /* [1/(g/cm2)^0.5]                        */
    const double X0     = 114.8;      /* g/cm^2                                 */
    const double MuMass = 0.105658;   /* Tomusim mu mass, GeV                   */
    const double Re     = 6371e3;     /* Earth radius, m                        */
    const double rho0   = 1.225e-3;   /* Dry air density at sea level, g/cm^3   */
    const double h0     = 6400.;      /* Atmosphere density length, m           */
    
    /* Corrections for altitude. */
    if (theta > 90.0)
        theta = 180.0-theta;

    double costheta, s0, Xz;
    {
        const double cz  = cos(theta*deg);
        const double sz2 = 1.-cz*cz;
        const double Rz  = Re+altitude;
    
        double r2 = Re/Rz;
	r2 *= r2;
        if (sz2 < r2)
	{
 	    s0       = Rz*cz-sqrt(Re*Re-Rz*Rz*sz2);
	    costheta = (Rz*cz-s0)/Re;
	    Xz       = rho0*h0*1e2/costheta*(1.0-exp(-s0*costheta/h0));
	}
 	else
	{
 	    s0       = 0.;
	    costheta = cz;
	    Xz       = 0.;
	}
    }
    
    /* Calculate cos(theta*). */
    const double costhetastar = _Chirkin_cos_thetastar(costheta);
    
    /* Calculate the effective initial energy, EI, and its derivative. */
    double EI, c0, c1, c2, dEIdEf;
    {
        const double X   = _Chirkin_mass_overburden(costheta)-X0-Xz;
		const double ebx = exp(b*X);
        const double Ef  = sqrt(momentum*momentum+MuMass*MuMass);
        const double Ei  = ((a+b*Ef)*ebx-a)/b;
 
 		c0 = astar-bstar*a/b;
 		c0 *= c0;
		c1 = (astar*astar-c0)/a;
 		c2 = bstar*bstar/b;
 		const double sE = 0.5*c2*(Ei*Ei-Ef*Ef)+c1*(Ei-Ef)+c0/b*log((a+b*Ei)/(a+b*Ef));
		const double dXf = astar+bstar*Ef;
		const double dXi = astar+bstar*Ei;
		const double dsEdEf = ebx*dXi*dXi/(a+b*Ei)-dXf*dXf/(a+b*Ef);
 		c0 = 1.1*costhetastar/115.;
 		c1 = 0.054;
 		c2 = 1.1*costhetastar/850.0;
		const double c01 = 1./(1.+c0*Ei);
		const double c21 = 1./(1.+c2*Ei);
		const double num1 = c0*c01*c01;
		const double num2 = c1*c2*c21*c21;
		const double num = num1+num2;
		const double iden = 1./(c01+c1*c21);
 		const double fr = -gamma/Ei-num*iden;
		const double dfrdEf = gamma/(Ei*Ei)+(num*num*iden+2.*(c0*num1*c01+c2*num2*c21))*iden;
 		EI = Ei+0.5*sE*fr;
		dEIdEf = fabs(ebx*(1.+0.5*dfrdEf*sE)+0.5*fr*dsEdEf);
    }
    
    /* Calculate survival factor (W). */
    double W;
    {
        const double d0 = _Chirkin_path_length(costheta);
 	W  = exp(-(d0-s0)/ctau*MuMass/EI);
    }
    
    /* Cut on magnetic rigidity. */
    if (p_latitude != NULL)
    {
        const double rigidity = _cutoff_rigidity_Stoermer(theta, phi, altitude, *p_latitude);
	const double pi = sqrt(EI*EI-MuMass*MuMass);
	W *= (pi >= rigidity);
    }	    
    
    /* Combine results. */
    return A0*W*dEIdEf*pow(EI, -gamma)*(1./(1.+c0*EI)+c1/(1.+c2*EI));
}

/* General purpose interface to a flux model. */
double differential_flux(int model, double momentum, double theta, double phi, double altitude, double* p_latitude)
{
    if (model == Chirkin)
        return _differential_flux_Chirkin(momentum, theta, phi, 0.0, NULL);
    else if (model == ModifiedChirkin)
        return _differential_flux_Chirkin(momentum, theta, phi, altitude, p_latitude);
    else
        return -1.;
}
