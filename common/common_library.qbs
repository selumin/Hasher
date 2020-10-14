import qbs

StaticLibrary {
    name: "hasher-common"

    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.network" }

    cpp.cxxLanguageVersion: "c++14"

    cpp.defines: []

    files: [
        "CommonDefinitions.h",
        "IPCFactory.h",
        "IPCInterface.h",
        "Logger.h",
        "Messages.cpp",
        "Messages.h",
        "QtSocketIPC.cpp",
        "QtSocketIPC.h",
        "Serializer.cpp",
        "Serializer.h",
        "Utils.cpp",
        "Utils.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [product.sourceDirectory]
   }
}

