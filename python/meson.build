project(
    'gauss2d',
    'cpp',
    default_options : ['cpp_std=c++17',],
    version : '0.1.0',
)
gauss2d_dep = dependency('gauss2d')
gauss2d_python_dep = dependency('gauss2d_python')
gauss2dfit_dep = dependency('gauss2dfit')
pybind11_dep = dependency('pybind11')
pymod = import('python')
python = pymod.find_installation(
    'python3',
    required : true,
    modules: ['gauss2d', 'pytest'],
)
subdir('gauss2d/fit')
subdir('tests')