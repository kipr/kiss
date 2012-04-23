import QtQuick 1.0

Image {
	property string imageSource
	property string title
	
	signal clicked()
	Component.onCompleted: {
		mouseArea.clicked.connect(clicked)
	}
	
	id: imageButton
	source: imageSource
	state: 'normal'
	
	MouseArea {
		id: mouseArea
		anchors.fill: parent
		hoverEnabled: true
		onEntered: parent.state = 'up'
		onExited: parent.state = 'normal'
	}
	
	Text {
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.bottom
		anchors.topMargin: 10
		text:title
	}
	
	states: [
		State {
			name: 'up'
			PropertyChanges { target:imageButton; y: -20 }
		},
		State {
			name: 'normal'
			PropertyChanges { target:imageButton; y: 0 }
		}
	]
	
	transitions: [
		Transition {
			from: 'normal'; to: 'up';
			NumberAnimation { properties: "x,y,z"; easing.type: Easing.InOutQuad }
		},
		Transition {
			from: 'up'; to: 'normal';
			SpringAnimation { properties: "x,y,z"; spring: 10; damping: 0.2 }
		}
	]
}