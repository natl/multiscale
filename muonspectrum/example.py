theta    = 0.
phi      = 0.
momentum = 0.20
altitude = 400.
latitude = None

from cFluxModel import differential_flux, MODIFIED_CHIRKIN
print differential_flux(model=MODIFIED_CHIRKIN, momentum=momentum, theta=theta, phi=phi, altitude=altitude, latitude=latitude)
"""
from flux import ModifiedChirkinFlux
flux = ModifiedChirkinFlux(altitude=altitude, latitude=latitude, pmin=momentum)
print flux.differential(theta = theta, phi = phi, momentum = momentum)
print flux.integrated(theta = theta, phi = phi)
"""
