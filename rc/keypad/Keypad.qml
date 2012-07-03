import Qt 4.7

Rectangle {
	id: wDialer
	color: "black"

	signal btnClick(string strText)
	signal btnDelClick

	Grid {
		id: layoutGrid
		anchors.fill: parent
		rows: 4; columns: 3

		DigitButton {
			mainText: "1"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "2"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "3"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "4"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "5"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "6"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "7"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "8"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "9"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "BK"
			onClicked: btnClick(strText)
			onDelClicked: btnDelClick()
		}
		DigitButton {
			mainText: "0"
			onClicked: btnClick(strText)
		}
		DigitButton {
			mainText: "CL"
			onClicked: btnClick(strText)
		}
	}
}