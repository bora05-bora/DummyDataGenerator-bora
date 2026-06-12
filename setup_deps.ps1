# nlohmann/json 단일 헤더 파일 다운로드 스크립트
# 사용법: PowerShell 에서 .\setup_deps.ps1 실행

$targetPath = "DummyDataGenerator\include\nlohmann\json.hpp"
$releaseUrl = "https://github.com/nlohmann/json/releases/latest/download/json.hpp"

if (Test-Path $targetPath) {
    Write-Host "[SKIP] $targetPath 이미 존재합니다." -ForegroundColor Yellow
    exit 0
}

Write-Host "nlohmann/json 헤더를 다운로드합니다..." -ForegroundColor Cyan

try {
    Invoke-WebRequest -Uri $releaseUrl -OutFile $targetPath -UseBasicParsing
    Write-Host "[OK] 다운로드 완료: $targetPath" -ForegroundColor Green
}
catch {
    Write-Host "[ERROR] 다운로드 실패: $_" -ForegroundColor Red
    Write-Host ""
    Write-Host "수동 설치 방법:" -ForegroundColor Yellow
    Write-Host "  1. https://github.com/nlohmann/json/releases 에서 json.hpp 다운로드"
    Write-Host "  2. DummyDataGenerator\include\nlohmann\ 폴더에 복사"
    exit 1
}
