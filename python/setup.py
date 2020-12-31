from setuptools import setup
with open("../README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()
setup(name='UniSerializer',
    version='1.0',
    description='Fast and efficient universal data serializer',
    url='https://github.com/Diffylab/UniSerializer',
    long_description=long_description,
    long_description_content_type="text/markdown",
    author='Diffy',
    author_email='diffylab@gmail.com',
    license='MIT',
    packages=['pkg'],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Topic :: Software Development :: Libraries"
        "Operating System :: OS Independent",
    ],
)