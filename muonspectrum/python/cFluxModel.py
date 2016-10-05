"""Interface to C flux models library.
"""

import os
from ctypes import byref as _byref, cdll as _cdll, c_double, c_int, c_void_p, POINTER as _POINTER

_path = os.path.dirname(__file__)
_path = os.path.abspath(os.path.join(_path, '..', 'lib', 'libflux.so.1.0.1'))

_libflux = _cdll.LoadLibrary(_path)

_libflux.differential_flux.argtypes = [c_int, c_double, c_double, c_double, c_double, _POINTER(c_double)]
_libflux.differential_flux.restype = c_double

def differential_flux(model, momentum, theta, phi=0., altitude=0., latitude=None):
    """Generic interface to a flux model.
    """
    
    if not (latitude is None):
        latitude = _byref(c_double(latitude))
	
    return _libflux.differential_flux(model, c_double(momentum), c_double(theta), c_double(phi), c_double(altitude), latitude)

"""
Available flux models.
"""
models = {
    'CHIRKIN'          : 0,
    'MODIFIED_CHIRKIN' : 1 
}
for _model, _index in models.iteritems():
    locals()[_model] = _index
