sources = [
    'centroidparameters.cc',
    'channel.cc',
    'component.cc',
    'data.cc',
    'ellipseparameters.cc',
    'ellipticalcomponent.cc',
    'fractionalintegralmodel.cc',
    'gaussiancomponent.cc',
    'integralmodel.cc',
    'linearintegralmodel.cc',
    'model.cc',
    'observation.cc',
    'param_filter.cc',
    'parameters.cc',
    'parametric.cc',
    'parametricmodel.cc',
    'psfmodel.cc',
    'source.cc',
    'pybind11.cc',
]

module = python.extension_module(
    '_gauss2d_fit', # Name of the module
    sources,  # List of sources
    install: true,  # Install it
    dependencies: [ # List of dependencies
        python.dependency(), # Add Python.h as dependency
        gauss2d_dep,
        gauss2d_python_dep,
        gauss2dfit_dep,
        pybind11_dep,
    ],
    subdir: 'gauss2d/fit', # Folder relative to site-packages to install to
)

# Pure Python sources
python_sources = [
    '__init__.py',
    'utils.py',
]

# Install pure Python
python.install_sources(
    python_sources,
    pure: false, # Will be installed next to binaries
    subdir: 'gauss2d/fit'  # Folder relative to site-packages to install to
)