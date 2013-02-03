# Links #

* http://www.terracoin.org/ - project main website
* http://sourceforge.net/projects/terracoin/ - sf mirror hosting files, code and soon discussion forum & eventually mailing lists.


# Development process #

Developers work in their own trees, then submit pull requests when
they think their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the
terracoin development team members simply pulls it.

Official Bitcoin patches are also regurlarly merged into Terracoin.

Newly developped features and additions are submitted to **dev-x.y.z** branch,
"x", "y" and "z" (respectively major, minor an build) matching a given Terracoin
milestone at github project page.

When an upcoming milestone is about to be released, code from given "dev-x.y.z"
branch is merged into corresponding "release-x.y.z" branch, for testing.

Upon validation, "release-x.y.z" branch is merged into "master" branch,
tested again, and a new tag is created.

Feature branches ("feature-shortname") may eventually be created when two
or more developpers works on the same task.


Testing
=======

Testing and code review is the bottleneck for development; we get more
pull requests than we can review and test. Please be patient and help
out, and remember this is a security-critical project where any
mistake might cost people lots of money.

Automated Testing
-----------------

Developers are strongly encouraged to write unit tests for new code,
and to submit new unit tests for old code.

Unit tests for the core code are in src/test/
To compile and run them:
  cd src; make -f makefile.linux test

Unit tests for the GUI code are in src/qt/test/
To compile and run them:
  qmake TERRACOIN_QT_TEST=1 -o Makefile.test bitcoin-qt.pro
  make -f Makefile.test
  ./Terracoin-Qt

