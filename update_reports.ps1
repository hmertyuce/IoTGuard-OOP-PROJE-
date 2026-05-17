$word = New-Object -ComObject Word.Application
$word.Visible = $false
$docsPath = "C:\Users\hmert\.gemini\antigravity\scratch\IoTGuard\docs"

$files = @("IoTGuard_Rapor_TR.docx", "IoTGuard_Report_EN.docx")

foreach ($file in $files) {
    $fullPath = Join-Path $docsPath $file
    if (Test-Path $fullPath) {
        $doc = $word.Documents.Open($fullPath)
        
        $findReplace = @{
            "Java" = "C++"
            ".java" = ".cpp"
            "JDT" = "MinGW"
            "JDK" = "MinGW"
            "Swing" = "Console"
            "java.util" = "<iostream>"
            "ArrayList" = "std::vector"
            "Interface" = "Abstract Class (Pure Virtual)"
            "abstract" = "virtual"
            "extends" = "public"
            "implements" = "public"
            "Exception" = "std::exception"
        }

        foreach ($key in $findReplace.Keys) {
            $value = $findReplace[$key]
            
            # Execute Replace (Find.Execute(FindText, MatchCase, MatchWholeWord, MatchWildcards, MatchSoundsLike, MatchAllWordForms, Forward, Wrap, Format, ReplaceWith, Replace))
            # Wrap = 1 (wdFindContinue)
            # Replace = 2 (wdReplaceAll)
            
            $find = $word.Selection.Find
            $find.Text = $key
            $find.Replacement.Text = $value
            $find.Execute([ref]$key, [ref]$true, [ref]$false, [ref]$false, [ref]$false, [ref]$false, [ref]$true, [ref]1, [ref]$false, [ref]$value, [ref]2) | Out-Null
        }
        
        $doc.Save()
        $doc.Close()
    }
}

$word.Quit()
echo "Word replace done"
