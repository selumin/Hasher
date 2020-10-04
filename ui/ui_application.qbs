import qbs

CppApplication {
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.network" }
    Depends { name: "hasher-common" }

    // Additional import path used to resolve QML modules in Qt Creator's code model
    property pathList qmlImportPaths: []

    cpp.cxxLanguageVersion: "c++14"

    cpp.defines: []

    files: [
        "Client.h",
        "Client.cpp",
        "Controller.cpp",
        "Controller.h",
        "main.cpp",
        "main.qml",
        "qml.qrc",
    ]

    Group {
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
