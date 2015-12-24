$Encoding = New-Object System.Text.UTF8Encoding($False)
foreach ($f in (dir -Recurse))
{
    if ($f.Extension -eq ".sln" -or $f.Extension -eq ".cs" -or $f.Extension -eq ".csproj" -or $f.Extension -eq ".vcxproj" -or $f.Extension -eq ".cpp")
    {
        $s = [System.IO.File]::ReadAllLines($f.FullName)
        [System.IO.File]::WriteAllLines($f.FullName, $s, $Encoding)
    }
}