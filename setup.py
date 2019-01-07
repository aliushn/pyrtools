#! /usr/bin/env python

from setuptools import setup, Extension

setup(
    name='pyrtools',
    version='0.1',
    description='Python tools for multi-scale image processing, including Laplacian pyramids, Wavelets, and Steerable Pyramids',
    license='MIT',
    url='https://github.com/LabForComputationalVision/pyrtools',
    author='Eero Simoncelli',
    author_email='eero.simoncelli@nyu.edu',
    keywords='multi-scale image-processing',
    packages=['pyrtools'],
    package_data={'': ['*.h', 'LICENSE']},
    install_requires=['numpy>=1.1',
                      'scipy>=0.18',
                      'matplotlib>=1.5',
                      'Pillow>=3.4',
                      'tqdm>=4.29',
                      'requests>=2.21'],
    ext_modules=[Extension('pyrtools.pyramids.c.wrapConv',
                           sources=['pyrtools/pyramids/c/convolve.c',
                                    'pyrtools/pyramids/c/edges.c',
                                    'pyrtools/pyramids/c/wrap.c',
                                    'pyrtools/pyramids/c/internal_pointOp.c'],
                           depends=['pyrtools/pyramids/c/convolve.h',
                                    'pyrtools/pyramids/c/internal_pointOp.h'],
                           extra_compile_args=['-fPIC', '-shared'])],
    tests='TESTS',
    )
