import QtQuick 1.0

Rectangle {
	id: root
	color: kissBackground;
	// opacity: 0.0
	
	Behavior on opacity { SmoothedAnimation { velocity:.9 } }

	Text {
		id: welcome
		anchors.topMargin: 20
		anchors.horizontalCenter: parent.horizontalCenter
		text: 'Welcome to KISS IDE'
		font.pointSize: 25
		y: 10
		opacity: 0.0
		
		Behavior on opacity { SmoothedAnimation { velocity:.9 } }
	}
	Flow {
		id: row
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter

		spacing: parent.width / 15

		ImageButton
		{
			imageSource: 'new.png'
			title: 'New File'
			onClicked: {
				kiss.newFile()
			}
		}
		ImageButton
		{
			imageSource: 'open.png'
			title: 'Open'
			onClicked: {
				kiss.open()
			}
		}
		ImageButton
		{
			imageSource: 'browser.png'
			title: 'Botball Community'
			onClicked: {
				kiss.openWeb("http://community.botball.org/")
			}
		}
		ImageButton
		{
			imageSource: 'settings.png'
			title: 'Settings'
			onClicked: {
				kiss.settings()
			}
		}
		
	}
	
	Connections {
		target: kiss
		onCompletedSetup: {
			tab.setTitle('Start Page')
			welcome.opacity = 1.0
			opacity = 1.0
		}
	}
}
