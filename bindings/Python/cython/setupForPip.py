import setuptools

with open("PIP.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
     name='libpointing',  
     version='1.0.8.2',
     provides=['libpointing'],
     python_requires="==3.9.*",
     package_data={'': ['libpointing.cpython-39-darwin.so', 'libpointing.cpython-39-x86_64-linux-gnu.so', 'libpointing.cp39-win_amd64.pyd']}, 
     author="Géry Casiez",
     author_email="gery.casiez@univ-lille.fr",
     description="Python binding for libpointing.",
     long_description=long_description,
     long_description_content_type="text/markdown",
     url="https://libpointing.org",
     packages=setuptools.find_packages(),
     install_requires=[ ],
     licence="GPLv2",
     platforms=['any'],

     classifiers=[
         "Programming Language :: Python :: 3",
         "License :: OSI Approved :: MIT License",
         "Operating System :: OS Independent",
         "License :: OSI Approved :: GNU General Public License v2 (GPLv2)"
     ],

 )