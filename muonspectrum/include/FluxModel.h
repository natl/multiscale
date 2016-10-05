#ifndef FLUX_MODEL_H__
#define FLUX_MODEL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Available flux models */
enum FluxModel {
   Chirkin         = 0,
   ModifiedChirkin = 1
};

/* General purpose interface to a flux model */
extern double differential_flux(int model, double momentum, double theta, double phi, double altitude, double* p_latitude);

#ifdef __cplusplus
}
#endif

#endif  // FluxModel.h
