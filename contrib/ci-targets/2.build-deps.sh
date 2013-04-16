#!/usr/bin/env bash
#
# 2.build-deps.sh :
#
# Build project, including dependencies for given platform (1st argument)
# (for a list of valid platforms, see 1.env-setup.sh)
#

RELEASE_VERSION="0.1.3"
SOURCE_DESTDIR=/home/terracoin/dependencies
RELEASE_PUBLISH_DIR=/home/terracoin/releases
TARGET_PLATFORMS=("mingw32")

exit_error() {
    echo $1;
    exit 1;
}

for CUR_PLATFORM in ${TARGET_PLATFORMS}; do
    if [ -z ${CUR_PLATFORM} ]; then
        exit_error "NO target platform given."
    fi

    # ensure platform base directory exist:
    platform_src_dir=${SOURCE_DESTDIR}/$CUR_PLATFORM
    [ -d ${platform_src_dir} ] || exit_error "INVALID platform given, or missing platformdir"

    # quite ugly case...
    case "${CUR_PLATFORM}" in
        mingw32)
            echo "Building dependencies for mingw32 platform..."

            # openssl
            # first check if we really want to rebuilt this (7 days old):
            need_rebuild=1
            if [ -f ${platform_src_dir}/openssl-1.0.1c/libcrypto.a ]; then
                echo "libcrypto.a already built, checking its oldness..."
                last_mtime=`stat -c "%Z" ${platform_src_dir}/openssl-1.0.1c/libcrypto.a`
                now_time=`date +"%s"`
                let now_time=now_time-604800
                if [ ${last_mtime} -gt ${now_time} ]; then
                    echo "libcrypto.a generated less than 7 days ago, not rebuilding..."
                    need_rebuild=0
                fi

            fi
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building openssl..."
                cd ${platform_src_dir}/openssl-1.0.1c/ || exit_error "Failed to change to openssl-1.0.1c/ dir"
                CROSS_COMPILE="i586-mingw32msvc-" ./Configure mingw no-asm no-shared --prefix=/usr/i586-mingw32msvc || exit_error "configure failed"
                PATH=$PATH:/usr/i586-mingw32msvc/bin make depend || exit_error "depend failed"
                PATH=$PATH:/usr/i586-mingw32msvc/bin make || exit_error "make failed"
                if [ ! -f ${platform_src_dir}/openssl-1.0.1c/libcrypto.a ]; then
                    exit_error "UNABLE TO FIND generated libcrypto.a"
                fi
            fi

            # berkeley DB
            need_rebuild=1
            if [ -f ${platform_src_dir}/db-4.8.30.NC/build_unix/libdb_cxx.a ]; then
                echo "libdb_cxx.a already built, checking its oldness..."
                last_mtime=`stat -c "%Z" ${platform_src_dir}/db-4.8.30.NC/build_unix/libdb_cxx.a`
                now_time=`date +"%s"`
                let now_time=now_time-604800
                if [ ${last_mtime} -gt ${now_time} ]; then
                    echo "libdb_cxx.a generated less than 7 days ago, not rebuilding..."
                    need_rebuild=0
                fi
            fi
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building libdb_cxx..."
                cd ${platform_src_dir}/db-4.8.30.NC/build_unix/ || exit_error "Failed to chainge to db-4.8.30.NC/build_unix/ dir"
                sh ../dist/configure --host=i586-mingw32msvc --enable-cxx --enable-mingw || exit_error "configure failed"
                make || exit_error "make failed"
                if [ ! -f ${platform_src_dir}/db-4.8.30.NC/build_unix/libdb_cxx.a ]; then
                    exit_error "UNABLE TO FIND generated libdb_cxx.a"
                fi
            fi

            # miniupnpc
            # not including for now...
            need_rebuild=0
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building miniupnpc..."
                cd ${platform_src_dir}/miniupnpc-1.6/ || exit_error "Failed to change to miniupnpc-1.6/ dir"
                sed -i 's/CC = gcc/CC = i586-mingw32msvc-gcc/' Makefile.mingw
                sed -i 's/wingenminiupnpcstrings \$/wine \.\/wingenminiupnpcstrings \$/' Makefile.mingw
                sed -i 's/dllwrap/i586-mingw32msvc-dllwrap/' Makefile.mingw
                sed -i 's/driver-name gcc/driver-name i586-mingw32msvc-gcc/' Makefile.mingw
                AR=i586-mingw32msvc-ar make -f Makefile.mingw
            fi
            [ -h ${platform_src_dir}/miniupnpc ] || ln -s ${platform_src_dir}/miniupnpc-1.6 ${platform_src_dir}/miniupnpc

            # boost
            need_rebuild=1
            if [ -f ${platform_src_dir}/boost_1_50_0/stage/lib/libboost_system-mt-s.a ]; then
                echo "libboost_system-mt.a already built, checking its oldness..."
                last_mtime=`stat -c "%Z" ${platform_src_dir}/boost_1_50_0/stage/lib/libboost_system-mt.a`
                now_time=`date +"%s"`
                let now_time=now_time-604800
                if [ ${last_mtime} -gt ${now_time} ]; then
                    echo "libboost_system-mt.a generated less than 7 days ago, not rebuilding..."
                    need_rebuild=0
                fi
            fi
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building boost..."
                cd ${platform_src_dir}/boost_1_50_0/ || exit_error "Failed to change to boost_1_50_0/ dir"
                ./bootstrap.sh --without-icu || exit_error "bootstrap failed"
                echo "using gcc : 4.4 : i586-mingw32msvc-g++ : <rc>i586-mingw32msvc-windres <archiver>i586-mingw32msvc-ar ;" > user-config.jam
                ./bjam toolset=gcc target-os=windows variant=release threading=multi threadapi=win32 link=static --user-config=user-config.jam -j 2 --without-mpi --without-python -sNO_BZIP2=1 -sNO_ZLIB=1 --layout=tagged --build-type=complete stage
            fi

            # qt
            need_rebuild=1
            if [ -f ${platform_src_dir}/qt/lib/libQtCore.a ]; then
                echo "libQtCore.a already built, checking its oldness..."
                last_mtime=`stat -c "%Z" ${platform_src_dir}/qt/lib/libQtCore.a`
                now_time=`date +"%s"`
                let now_time=now_time-604800
                if [ ${last_mtime} -gt ${now_time} ]; then
                    echo "libQtCore.a generated less than 7 days ago, not rebuilding..."
                    need_rebuild=0
                fi
            fi

            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building qt..."
                cd ${platform_src_dir}/qt-everywhere-opensource-src-4.8.3/ || exit_error "Failed to change to qt source dir"
                sed 's/$TODAY/2011-01-30/' -i configure
                sed 's/i686-pc-mingw32-/i586-mingw32msvc-/' -i mkspecs/unsupported/win32-g++-cross/qmake.conf
                sed --posix 's|QMAKE_CFLAGS\t\t= -pipe|QMAKE_CFLAGS\t\t= -pipe -isystem /usr/i586-mingw32msvc/include/ -frandom-seed=qtbuild|' -i mkspecs/unsupported/win32-g++-cross/qmake.conf
                sed 's/QMAKE_CXXFLAGS_EXCEPTIONS_ON = -fexceptions -mthreads/QMAKE_CXXFLAGS_EXCEPTIONS_ON = -fexceptions/' -i mkspecs/unsupported/win32-g++-cross/qmake.conf
                sed 's/QMAKE_LFLAGS_EXCEPTIONS_ON = -mthreads/QMAKE_LFLAGS_EXCEPTIONS_ON = -lmingwthrd/' -i mkspecs/unsupported/win32-g++-cross/qmake.conf
                sed --posix 's/QMAKE_MOC\t\t= i586-mingw32msvc-moc/QMAKE_MOC\t\t= moc/' -i mkspecs/unsupported/win32-g++-cross/qmake.conf
                sed --posix 's/QMAKE_RCC\t\t= i586-mingw32msvc-rcc/QMAKE_RCC\t\t= rcc/' -i mkspecs/unsupported/win32-g++-cross/qmake.conf
                sed --posix 's/QMAKE_UIC\t\t= i586-mingw32msvc-uic/QMAKE_UIC\t\t= uic/' -i mkspecs/unsupported/win32-g++-cross/qmake.conf

                [ -d ${platform_src_dir}/qt ] || mkdir ${platform_src_dir}/qt
                ./configure -prefix ${platform_src_dir}/qt -confirm-license -release -opensource -static -no-qt3support -xplatform unsupported/win32-g++-cross -no-multimedia -no-audio-backend -no-phonon -no-phonon-backend -no-declarative -no-script -no-scripttools -no-javascript-jit -no-webkit -no-svg -no-xmlpatterns -no-sql-sqlite -no-nis -no-cups -no-iconv -no-dbus -no-gif -no-libtiff -no-opengl -nomake examples -nomake demos -nomake docs -no-feature-style-plastique -no-feature-style-cleanlooks -no-feature-style-motif -no-feature-style-cde -no-feature-style-windowsce -no-feature-style-windowsmobile -no-feature-style-s60 || exit_error "configure failed"
                make || exit_error "make failed"
                make install || exit_error "make install failed"


            fi

            # release build ?
            if [ "${GIT_BRANCH}" == "master" ]; then
                do_release_build=1
            else
                do_release_build=0
            fi

            # qt client:
            echo "Building terracoin qt client..."
            cd ${WORKSPACE} || exit_error "Failed to change to workspace dir"
            make distclean
            PATH=${platform_src_dir}/qt/bin:$PATH ${platform_src_dir}/qt/bin/qmake -spec unsupported/win32-g++-cross MINIUPNPC_LIB_PATH=${platform_src_dir}/miniupnpc-1.6 MINIUPNPC_INCLUDE_PATH=${platform_src_dir} BDB_LIB_PATH=${platform_src_dir}/db-4.8.30.NC/build_unix BDB_INCLUDE_PATH=${platform_src_dir}/db-4.8.30.NC/build_unix BOOST_LIB_PATH=${platform_src_dir}/boost_1_50_0/stage/lib BOOST_INCLUDE_PATH=${platform_src_dir}/boost_1_50_0 BOOST_LIB_SUFFIX=-mt BOOST_THREAD_LIB_SUFFIX=_win32-mt OPENSSL_LIB_PATH=${platform_src_dir}/openssl-1.0.1c OPENSSL_INCLUDE_PATH=${platform_src_dir}/openssl-1.0.1c/include QRENCODE_LIB_PATH=${platform_src_dir}/qrencode-3.2.0/.libs QRENCODE_INCLUDE_PATH=${platform_src_dir}/qrencode-3.2.0 USE_UPNP=1 USE_QRCODE=0 INCLUDEPATH=${platform_src_dir} DEFINES=BOOST_THREAD_USE_LIB QMAKE_LRELEASE=lrelease QMAKE_CXXFLAGS=-frandom-seed=terracoin QMAKE_LFLAGS=-frandom-seed=terracoin USE_BUILD_INFO=1 TERRACOIN_NEED_QT_PLUGINS=1 RELEASE=${do_release_build} || exit_error "qmake failed"
            PATH=${platform_src_dir}/qt/bin:$PATH make || exit_error "Make failed"

            # terracoin headless daemon:
            echo "Building terracoin headless daemon..."
            cd ${WORKSPACE}/src/ || exit_error "Failed to change to terracoin src/"
            make -f makefile.linux-mingw clean
            cd ${WORKSPACE}/src/leveldb/ || exit_error "Failed to change to src/leveldb/"
            PATH=/usr/i586-mingw32msvc/bin/:$PATH TARGET_OS="OS_WINDOWS_CROSSCOMPILE" CXX=i586-mingw32msvc-c++ CC=i586-mingw32msvc-cc LD=i586-mingw32msvc-ld OPT="-I${platform_src_dir}/boost_1_50_0" make libmemenv.a libleveldb.a || exit_error "Failed to build leveldb"
            cd ${WORKSPACE}/src/ || exit_error "Failed to change to src/"
            export MINGW_EXTRALIBS_DIR=${platform_src_dir}
            make -f makefile.linux-mingw || exit_error "make failed"
            /usr/i586-mingw32msvc/bin/strip terracoind.exe || exit_error "strip failed"
            [ -f ${WORKSPACE}/src/terracoind.exe ] || exit_error "UNABLE to find generated terracoind.exe"
            echo "terracoind compile success."

            # copy built files if built branch is 'master':
            #if [ "${GIT_BRANCH}" == "master" ]; then
                release_out_dir=${RELEASE_PUBLISH_DIR}/${CUR_PLATFORM}

                jobname="terracoin"
                if [ "${JOB_NAME}" == "terracoin-dev" ]; then
                    jobname="dev"
                elif [ "${JOB_NAME}" == "terracoin-release" ]; then
                    jobname="release"
                fi

                [ -d ${release_out_dir} ] || mkdir -p ${release_out_dir}
                if [ ! -d ${release_out_dir} ]; then
                    echo "UNABLE to create release_out_dir="${release_out_dir}
                else
                    /bin/cp -f ${WORKSPACE}/src/terracoind.exe ${release_out_dir}/ || exit_error "FAILED to copy terracoind.exe"
                    /bin/cp -f ${WORKSPACE}/release/terracoin-qt.exe ${release_out_dir}/ || exit_error "FAILED to copy terracoin-qt.exe"
                    /bin/cp ${WORKSPACE}/{INSTALL,COPYING,README.md} ${release_out_dir}/ || exit_error "FAILED to copy text files"
                    cd ${release_out_dir}/
                    /usr/bin/zip -v -9 ${jobname}-${RELEASE_VERSION}-${BUILD_NUMBER}-win32.zip COPYING INSTALL README.md terracoind.exe terracoin-qt.exe || exit_error "FAILED to create zip archive."
                    echo "ZIP archive created."
                fi
            #fi


        ;;

        *)
            exit_error "Not Yet Implemented"
        ;;
    esac

done
