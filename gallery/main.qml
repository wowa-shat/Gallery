import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.0
import Model 1.0
import MyLang 1.0

Window {
    id: window
    visible: true
    width: 720
    height: 720
    title: qsTr("Галлерея") + trans.emptyString

    //меню
    MenuBar {
        Menu {
            //BUG: не переводится пока не навёл курсор
            title: qsTr("Меню") + trans.emptyString

            MenuItem {
                id: chooseFolder
                text: qsTr("Выберите папку") + trans.emptyString
                onTriggered: folderDialog.visible = true
            }

            MenuItem {
                id: removeImage
                text: qsTr("Удалить изображение") + trans.emptyString
                onTriggered :
                {
                    if (img.source != "") {
                        imageModel.removeItem(img.source)
                        img.source = ""
                        imgBack.source = ""
                    }
                }
            }

            Menu {
                title: qsTr("Выбор языка") + trans.emptyString
                MenuItem {
                    text: qsTr("Русский") + trans.emptyString
                    onTriggered :
                    {
                        trans.updateLanguage(MyLang.RUS)
                    }
                }
                MenuItem {
                    text: qsTr("Английский") + trans.emptyString
                    onTriggered :
                    {
                        trans.updateLanguage(MyLang.ENG)
                    }
                }
            }
        }
    }

    //выезжающая менюшка с информацией о файле
    Drawer {
        id: info
        width: 0.40 * window.width
        height: window.height - allPhoto.height

        Column {
            spacing: 10
            anchors.centerIn: parent
            Text { id: name; text: qsTr("Имя файла: ") + trans.emptyString + FileManager.fileInfo["name"]}
            Text { id: size; text: qsTr("Размер файла: ") + trans.emptyString + FileManager.fileInfo["size"]}
            Text { id: birth; text: qsTr("Дата создания: ") + trans.emptyString + FileManager.fileInfo["birth"]}
            Text { id: mod; text: qsTr("Последнее изменение: ") + trans.emptyString + FileManager.fileInfo["mod"]}
        }
    }

    FolderDialog {
        id: folderDialog
        visible: false
        onAccepted: {
            imageModel.scanFolder(folderDialog.folder)
        }
    }

    //выбранное в данный момент фото с его фоном
    Item {
        id: currentPhoto
        width: parent.width
        anchors.bottom: allPhoto.top
        anchors.top: parent.top

        //фон
        Image {
            id:imgBack
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectCrop
            width: parent.width - 40
            height: parent.height - 40
        }

        //размытие фона
        FastBlur {
            anchors.fill: imgBack
            source: imgBack
            radius: 32
        }

        //основа
        Image {
            id:img
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
            width: parent.width - 40
            height: parent.height - 40
            transform: Translate {
                x: info.position * img.width * 0.33
            }
            onSourceChanged:
            {
                FileManager.changeInfo(img.source)
            }
        }
    }

    Item {
        id: allPhoto
        width: parent.width
        height: 150
        anchors.bottom: parent.bottom

        Model {
            id: imageModel
        }

        ListView
        {
            id: listView
            delegate: imageDelegate
            model: imageModel
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.topMargin: 10
            spacing: 10
            orientation: Qt.Horizontal
        }

        Component {
            id: imageDelegate
            Item {
                width: 100
                height: listView.height
                clip: true
                Column {
                    Text {
                        id: imageName
                        text: name
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        elide: Text.ElideRight
                        MouseArea {
                            id: clickText
                            anchors.fill: imageName
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                input.visible = true
                                imageName.visible = false
                            }
                        }
                    }
                    TextInput {
                        id: input
                        visible: false
                        width: imageName.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: imageName.text
                        cursorVisible: false
                        onFocusChanged: {
                            if (focus == true)
                                input.color = "#5c55b7"
                            else {
                                input.visible = false
                                imageName.visible = true
                                input.color = "black"
                            }
                        }
                        onEditingFinished: {
                            input.visible = false
                            imageName.visible = true
                            if (!imageModel.renameFile(model.index, text)) text = imageName.text
                            input.focus = false
                        }
                    }

                    Image {
                        id: clickableImage
                        width: 100
                        height: 100
                        fillMode: Image.PreserveAspectCrop
                        source: url
                        MouseArea {
                            id: clickImage
                            anchors.fill: clickableImage
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                img.source = clickableImage.source
                                imgBack.source = clickableImage.source
                            }
                        }
                    }
                }
            }
        }
    }
}
