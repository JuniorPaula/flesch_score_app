package main

/*
#cgo LDFLAGS: -L../readability/target/release -lreadability
#include <stdlib.h>

// Função da lib Rust para calcular o flesch score
extern double flesch_score(const char* text, size_t len);

// Função da lib Rust para calcular as métricas
typedef struct {
 size_t sentences;
 size_t words;
 size_t syllables;
} TextStats;

extern TextStats get_text_stats(const char* text, size_t len);
*/
import "C"

import (
	"fmt"
	"image/color"
	"os"
	"strings"
	"unsafe"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/dialog"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
)

var legend *fyne.Container

type Stats struct {
	Sentences int
	Words     int
	Syllables int
}

var (
	currentScore float64
	currentLevel string
	currentStats Stats
)

func calculateScore(text string) float64 {
	cstr := C.CString(text)
	defer C.free(unsafe.Pointer(cstr))

	return float64(C.flesch_score(cstr, C.size_t(len(text))))
}

func getStats(text string) Stats {
	cstr := C.CString(text)
	defer C.free(unsafe.Pointer(cstr))

	s := C.get_text_stats(cstr, C.size_t(len(text)))

	return Stats{
		Sentences: int(s.sentences),
		Words:     int(s.words),
		Syllables: int(s.syllables),
	}
}

func classifyScore(score float64) (string, color.Color) {
	switch {
	case score >= 90:
		return "Muito fácil (crianças/ensino fundamental)", DARKGREEN
	case score >= 75:
		return "Fácil (leitura rápida)", LIGHTGREEN
	case score >= 50:
		return "Média (adequado ao público geral)", YELLOW
	case score >= 25:
		return "Difícil (text técnico)", ORANGE
	default:
		return "Muito difícil (especialistas ou acadêmicos)", RED
	}
}

func generateInsight(level string) string {
	switch {
	case strings.Contains(level, "Muito fácil"):
		return "Texto extremamente acessível. Ideal para crianças ou leitores com baixa escolaridade."
	case strings.Contains(level, "Fácil"):
		return "Texto leve e bem compreensível. Perfeito para blogs, newletters e conteúdos amplos."
	case strings.Contains(level, "Média"):
		return "Texto com legibilidade moderada. Adequado para o público geral com leitura fluente."
	case strings.Contains(level, "Difícil"):
		return "Texto denso. Pode exigir maior atenção ou conhecimento prévio do leitor."
	case strings.Contains(level, "Muito dificil"):
		return "Texto técnico ou acadêmico. Recomendado para especialistas ou leitores avançados."
	default:
		return ""
	}
}

var (
	DARKGREEN  = color.RGBA{R: 0x00, G: 0xcc, B: 0x00, A: 0xff}
	LIGHTGREEN = color.RGBA{R: 0x66, G: 0xff, B: 0x66, A: 0xff}
	YELLOW     = color.RGBA{R: 0xff, G: 0xd7, B: 0x00, A: 0xff}
	ORANGE     = color.RGBA{R: 0xff, G: 0xa5, B: 0x00, A: 0xff}
	RED        = color.RGBA{R: 0xff, G: 0x45, B: 0x00, A: 0xff}
)

func makeLegend(level string) *fyne.Container {
	type legendItem struct {
		label string
		color color.Color
	}

	levels := []legendItem{
		{"Muito fácil", DARKGREEN},
		{"Fácil", LIGHTGREEN},
		{"Média", YELLOW},
		{"Difícil", ORANGE},
		{"Muito difícil", RED},
	}

	var items []fyne.CanvasObject

	for _, l := range levels {
		text := canvas.NewText("⬤ "+l.label, l.color)
		text.Alignment = fyne.TextAlignLeading
		if strings.Contains(level, l.label) {
			text.TextStyle = fyne.TextStyle{Bold: true}
		} else {
			text.Color = color.Gray{Y: 100}
		}

		items = append(items, text)
	}

	return container.NewVBox(items...)
}

func formatScore(score float64) string {
	return fmt.Sprintf("%.2f", score)
}

func main() {
	app := app.NewWithID("flesch.score.app")
	win := app.NewWindow("Flesch Score App")

	// Widgets de informação
	scoreLabel := widget.NewLabel("Flesch Score:")
	levelLabel := widget.NewLabel("Classificação:")

	// insight
	insightLabel := widget.NewLabel("")

	// Progressbar
	progressBar := canvas.NewRectangle(color.Gray{Y: 180})
	progressBar.SetMinSize(fyne.NewSize(0, 24))
	progressLabel := canvas.NewText("0%", color.Black)
	progressLabel.Alignment = fyne.TextAlignCenter
	progressLabel.TextStyle = fyne.TextStyle{Bold: true}

	progressContainer := container.NewMax(
		progressBar,
		container.NewCenter(progressLabel),
	)

	// Pré-visualização do texto
	textPreview := widget.NewMultiLineEntry()
	textPreview.SetPlaceHolder("Prévia do texto selecionado...")
	textPreview.SetMinRowsVisible(10)

	selectFileButton := widget.NewButton("Selecionar Arquivo", func() {
		dialog.NewFileOpen(func(reader fyne.URIReadCloser, err error) {
			if err != nil || reader == nil {
				return
			}

			data, err := os.ReadFile(reader.URI().Path())
			if err != nil {
				scoreLabel.SetText("Erro ao ler o arquivo.")
				return
			}

			textPreview.SetText(string(data))

			// TODO: call func to get text score
			score := calculateScore(string(data))
			scoreLabel.SetText("Flesch Score: " + formatScore(score))

			level, fillColor := classifyScore(score)
			levelLabel.SetText("Classificação: " + level)

			progressBar.FillColor = fillColor
			progressLabel.Text = formatScore(score)
			progressBar.Refresh()

			currentStats = getStats(string(data))
			currentScore = score
			currentLevel = level

			insight := generateInsight(level)
			insightLabel.SetText(" Insight: " + insight)

			legend.Objects = makeLegend(level).Objects
		}, win).Show()
	})

	// Botão de exportação de métricas
	exportButton := widget.NewButtonWithIcon("Exportar Relatório", theme.DocumentSaveIcon(), func() {})

	// Top com botão
	top := container.NewBorder(nil, nil, nil, exportButton, selectFileButton)

	// Legenda no rodapé
	legend = makeLegend("")

	// Rodapé com análise
	bottom := container.NewVBox(
		scoreLabel,
		levelLabel,
		insightLabel,
		progressContainer,
		legend,
	)

	layout := container.NewBorder(
		top,
		bottom,
		nil,
		nil,
		container.NewScroll(textPreview),
	)

	win.SetContent(layout)
	win.Resize(fyne.NewSize(1024, 680))
	win.ShowAndRun()
}
