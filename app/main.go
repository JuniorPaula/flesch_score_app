package main

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/widget"
)

func main() {
	app := app.NewWithID("flesch.score.app")
	win := app.NewWindow("Flesch Score App")

	win.SetContent(widget.NewLabel("Hello From Fyne App!"))
	win.Resize(fyne.NewSize(600, 400))
	win.ShowAndRun()
}
