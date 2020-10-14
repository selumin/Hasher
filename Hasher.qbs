import qbs

Project {
    name: "Hasher"

    SubProject {
        filePath: "common/common_library.qbs";
        Properties {
            name: "Common library"
        }
    }

    SubProject {
        filePath: "ui/ui_application.qbs";
        Properties {
            name: "Hasher UI Application"
        }
    }

    SubProject {
        filePath: "server/server_application.qbs";
        Properties {
            name: "Hasher Server Application"
        }
    }
}
