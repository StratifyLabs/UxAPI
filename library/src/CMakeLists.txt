
set(SOURCES

	# Utility and Interface
	Component.cpp
	Event.cpp
	EventLoop.cpp
	TouchGesture.cpp
	Controller.cpp
	Model.cpp

	# Components
	Label.cpp
	Button.cpp
	Progress.cpp
	ProgressBar.cpp
	ToggleSwitch.cpp
	Slider.cpp
	List.cpp
	Layout.cpp
	Graph.cpp
	Separator.cpp
	TextArea.cpp

	#Drawing primitives
	Drawing.cpp
	draw/Icon.cpp
	draw/Image.cpp
	draw/Text.cpp
	draw/RichText.cpp
	draw/TextBox.cpp
	draw/Rectangle.cpp

	sgfx/Api.cpp
	sgfx/Area.cpp
	sgfx/Cursor.cpp
	sgfx/Font.cpp
	sgfx/IconFont.cpp
	sgfx/Pen.cpp
	sgfx/Bitmap.cpp
	sgfx/Point.cpp
	sgfx/Theme.cpp
	sgfx/Palette.cpp
	sgfx/Vector.cpp

	PARENT_SCOPE

	)
