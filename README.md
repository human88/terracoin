
Development process
===================

Developers work in their own trees, then submit pull requests when
they think their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the
terracoin development team members simply pulls it.

Official Bitcoin patches are also regurlarly merged into Terracoin.

Newly developped features and additions are submitted to **dev-x.y.z** branch,
"x", "y" and "z" (respectively major, minor an build) matches a given Terracoin
milestone at github project page.

When an upcoming milestone is about to be released, code from given "dev-x.y.z"
branch is merged into corresponding "release-x.y.z" branch, for testing.

Upon validation, "release-x.y.z" branch is merged into "master" branch, and a
new tag is created.

Feature branches ("feature-shortname") may eventually be created when two
or more developpers works on a common feature.

