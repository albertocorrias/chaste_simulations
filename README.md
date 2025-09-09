# A simple repository to store Chaste simulations

Initially, this was cretaed following the [Chaste project template](https://github.com/Chaste/template_project), as [recommended](https://chaste.github.io/docs/user-guides/user-projects/).

This is intended to be cloned into `Chaste/projects/` where `Chaste/` contains the Chaste source code (cloned or downloaded). 

For new simulations

 - Add a file `Testxxxxx.hpp` to the `test` directory containing the simulation code
 - Add `Testxxxxx.hpp` to `ContinuousTestPack.txt` to make it visible to the cmake infrastructure.
 - Navigate to the `chaste_build` directory: this is the one where Chaste was built after downloading (see [configure and build steps](https://chaste.github.io/docs/user-guides/cmake-first-run/) )
 - Type `ccmake /path/to/chaste_source_code_directory` (the same as `Chaste/` mentioned above). Type `c` to configure, then exit with `e`, then `g` to generate
 - Then the target should be available: `make Testxxxxx` should prepare the executable
 - Run with `ctest -R Testxxxxx`
