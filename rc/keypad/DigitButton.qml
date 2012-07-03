import Qt 4.7

Item {
	id: digButton
	width: ((parent.width / layoutGrid.columns) - parent.spacing);
	height: ((parent.height / layoutGrid.rows) - parent.spacing);

	property string mainText: "2"
	property bool isDel: false

	signal clicked(string strText)
	signal delClicked()

	MyButton {
		id: btn
		mainText: digButton.mainText
		anchors.fill: parent
		mainPixelSize: parent.height - 6
		z: 1

		onClicked: {
			if (digButton.isDel) digButton.delClicked();
			else digButton.clicked(mainText.substring(0, 1));
		}

		onPressHold: {
			if ((mainText.length > 1) && (!digButton.isDel)) {
				digButton.clicked(mainText.substring(1, 2));
			} else {
				if (digButton.isDel) digButton.delClicked();
				else digButton.clicked(mainText.substring(0,1));
			}
		}
	}
}