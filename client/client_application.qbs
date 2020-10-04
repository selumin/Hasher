import qbs

CppApplication {
    Depends { name: "Qt.core" }
    Depends { name: "Qt.network" }
    Depends { name: "hasher-common" }

    // Additional import path used to resolve QML modules in Qt Creator's code model
    property pathList qmlImportPaths: []

    cpp.cxxLanguageVersion: "c++14"

    cpp.defines: []

    files: [
        "Client.cpp",
        "Client.h",
        "main.cpp",
    ]

    Group {
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
