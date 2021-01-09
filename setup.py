from setuptools import setup
setup(name='uniserializer',
    version='1.25',
    description='Fast and efficient universal data serializer',
    url='https://github.com/Diffylab/UniSerializer',
    author='Diffy',
    author_email='diffylab@gmail.com',
    download_url = 'https://github.com/Diffylab/UniSerializer/releases/',
    keywords = ['Serializer', 'Deserializer', 'data', 'binary', 'encoding', 'schema',' schemaless'],
    license='MIT',
    package_dir={'uniserializer': 'python'},
    packages=['uniserializer'],
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Topic :: Software Development :: Libraries',
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Natural Language :: English',
        'Operating System :: OS Independent',
        'Topic :: Software Development :: Libraries :: Python Modules'
    ]
)
