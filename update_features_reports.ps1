$word = New-Object -ComObject Word.Application
$word.Visible = $false
$docsPath = "C:\Users\hmert\.gemini\antigravity\scratch\IoTGuard\docs"

# TR Report
$trPath = Join-Path $docsPath "IoTGuard_Rapor_TR.docx"
if (Test-Path $trPath) {
    $doc = $word.Documents.Open($trPath)
    $selection = $word.Selection
    $selection.EndKey(6) | Out-Null
    $selection.TypeParagraph()
    $selection.Font.Bold = 1
    $selection.TypeText("SON GÜNCELLEMELER (V2.0 C++ EDITION):")
    $selection.Font.Bold = 0
    $selection.TypeParagraph()
    $selection.TypeText("- Proje yapısı karmaşıklığı önlemek adına Tek-Dosya (main.cpp) mimarisine geçirilmiştir.")
    $selection.TypeParagraph()
    $selection.TypeText("- ANSI renk kodları ve daktilo (typing) animasyonları ile gelişmiş interaktif TUI (Metin Tabanlı Arayüz) entegre edilmiştir.")
    $selection.TypeParagraph()
    $selection.TypeText("- Yönetici (Admin) Giriş paneli eklenmiş ve şifre maskeleme özellikleri uygulanmıştır.")
    $selection.TypeParagraph()
    $selection.TypeText("- Multi-Threaded tarama animasyonları (Progress Bar) ve Matrix dijital yağmur efekti sisteme dahil edilmiştir.")
    $doc.Save()
    $doc.Close()
}

# EN Report
$enPath = Join-Path $docsPath "IoTGuard_Report_EN.docx"
if (Test-Path $enPath) {
    $doc = $word.Documents.Open($enPath)
    $selection = $word.Selection
    $selection.EndKey(6) | Out-Null
    $selection.TypeParagraph()
    $selection.Font.Bold = 1
    $selection.TypeText("LATEST UPDATES (V2.0 C++ EDITION):")
    $selection.Font.Bold = 0
    $selection.TypeParagraph()
    $selection.TypeText("- Project structure has been simplified to a Single-File (main.cpp) architecture to prevent complexity.")
    $selection.TypeParagraph()
    $selection.TypeText("- Advanced interactive TUI (Text User Interface) integrated with ANSI color codes and cinematic typing animations.")
    $selection.TypeParagraph()
    $selection.TypeText("- Administrator Login panel added with dynamic password masking (*).")
    $selection.TypeParagraph()
    $selection.TypeText("- Multi-Threaded scanning animations (Progress Bar) and a digital rain Matrix visual effect have been implemented.")
    $doc.Save()
    $doc.Close()
}

$word.Quit()
echo "Raporlar guncellendi."
