import Qt 4.7

Rectangle {
	id: button
	border.color: activeFocus ? "orange" : "white"
	color: "#202020"
	smooth: true

	radius: ((height + width) / 20);
	height: mText.height + 4

	property string mainText: "2"
	property real mainPixelSize: 100

	property alias aliasTextElide: mText.elide

	signal clicked(string strText)
	signal pressHold(string strText)

	onWidthChanged: updateTextWidth();
	function updateTextWidth() {
		if (mText.elide != Text.ElideNone) mText.width = button.width
	}

	Text {
		id: mText
		text: button.mainText
		color: "white"
		
		onTextChanged: button.updateTextWidth();

		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter

		font.pixelSize: mainPixelSize
	}

	MouseArea {
		id: mouseArea
		anchors.fill: parent

		onClicked: button.clicked(mainText);
		onPressAndHold: button.pressHold(mainText);
	}

	Keys.onReturnPressed: button.clicked(mainText);
	Keys.onSpacePressed: button.clicked(mainText);

	states: State {
		name: "pressed"
		when: mouseArea.pressed
		PropertyChanges { target: button; color: "orange" }
		PropertyChanges { target: mText; color: "black" }
	}
}