# DummyDataGenerator

[DataMonitor-bora](https://github.com/bora05-bora/DataMonitor-bora)의 JSON 주문 데이터 스키마를 기반으로 대량의 더미 데이터를 생성하는 C++ POC입니다.

## 개요

DataMonitor 시스템에서 사용하는 `orders.json` / `samples.json` 형식과 동일한 구조의 더미 데이터를 생성합니다.  
생성 수량, 날짜 범위, 랜덤 시드 등을 CLI 인수로 제어할 수 있습니다.

## 출력 데이터 구조

### orders_dummy.json

```json
{
  "orders": [
    {
      "id": 1,
      "sample": "Alpha-001",
      "quantity": 87,
      "status": "Pending",
      "date": "2026-03-15"
    }
  ]
}
```

| 필드 | 타입 | 설명 |
|------|------|------|
| `id` | int | 순번 (1부터 시작) |
| `sample` | string | 샘플 이름 (예: Alpha-001) |
| `quantity` | int | 주문 수량 |
| `status` | string | `Pending` / `Processing` / `Completed` / `Cancelled` |
| `date` | string | 주문 날짜 (YYYY-MM-DD) |

### samples_dummy.json

```json
{
  "samples": [
    {
      "id": 1,
      "name": "Alpha-001",
      "stock": 187,
      "productionTime": 12.5,
      "yield": 86.5
    }
  ]
}
```

| 필드 | 타입 | 설명 |
|------|------|------|
| `id` | int | 순번 (1부터 시작) |
| `name` | string | 샘플 이름 |
| `stock` | int | 재고 수량 |
| `productionTime` | double | 생산 소요 시간 (시간 단위) |
| `yield` | double | 수율 (%) |

## 빌드 환경

| 항목 | 버전 |
|------|------|
| IDE | Visual Studio |
| PlatformToolset | v145 |
| 표준 | C++17 |
| 플랫폼 | x64 |
| 외부 라이브러리 | [nlohmann/json](https://github.com/nlohmann/json) |

## 빌드 방법

### 1. 의존성 설치

`nlohmann/json` 헤더 파일이 포함되어 있습니다.  
포함되어 있지 않은 경우 PowerShell 스크립트로 다운로드하세요.

```powershell
.\setup_deps.ps1
```

### 2. Visual Studio에서 빌드

`DummyDataGenerator.sln`을 열고 **빌드 > 솔루션 빌드** (`Ctrl+Shift+B`)

### 3. MSBuild로 빌드

```powershell
msbuild DummyDataGenerator.sln /p:Configuration=Release /p:Platform=x64
```

빌드 결과물: `bin\Release\DummyDataGenerator.exe`

## 실행 방법

```
DummyDataGenerator.exe [옵션]
```

### CLI 옵션

| 옵션 | 기본값 | 설명 |
|------|--------|------|
| `--orders=N` | `100` | 생성할 주문 레코드 수 |
| `--samples=N` | `10` | 생성할 샘플 레코드 수 |
| `--start=YYYY-MM-DD` | `2026-01-01` | 날짜 범위 시작 |
| `--end=YYYY-MM-DD` | `2026-12-31` | 날짜 범위 끝 |
| `--seed=N` | `0` | 랜덤 시드 (0이면 시간 기반 자동 생성) |
| `--out=경로` | `output` | 출력 디렉토리 |
| `--min-qty=N` | `1` | 주문 수량 최솟값 |
| `--max-qty=N` | `200` | 주문 수량 최댓값 |

### 예시

```powershell
# 기본 실행 (주문 100건, 샘플 10개)
.\DummyDataGenerator.exe

# 주문 1000건, 샘플 20개, 재현 가능한 시드 사용
.\DummyDataGenerator.exe --orders=1000 --samples=20 --seed=42

# 날짜 범위 및 출력 경로 지정
.\DummyDataGenerator.exe --orders=500 --start=2025-01-01 --end=2025-12-31 --out=C:\data
```

### 실행 예시 출력

```
=== DummyDataGenerator ===
seed    : 42
samples : 5
orders  : 20
range   : 2026-01-01 ~ 2026-06-30

[OK] samples -> "output\samples_dummy.json"
[OK] orders  -> "output\orders_dummy.json"

--- Status distribution ---
  Pending    : 9 (45.0%)
  Processing : 5 (25.0%)
  Completed  : 4 (20.0%)
  Cancelled  : 2 (10.0%)

Done.
```

## 프로젝트 구조

```
DummyDataGenerator/
├── DummyDataGenerator.sln
├── setup_deps.ps1                      # nlohmann/json 다운로드 스크립트
└── DummyDataGenerator/
    ├── DummyDataGenerator.vcxproj
    ├── DummyDataGenerator.vcxproj.filters
    ├── include/
    │   └── nlohmann/json.hpp           # 단일 헤더 JSON 라이브러리
    ├── data/
    │   └── orders_template.json        # DataMonitor-bora 원본 샘플 데이터
    └── src/
        ├── main.cpp                    # 진입점 및 CLI 파싱
        ├── Config.h                    # 생성 파라미터 구조체
        ├── DataModels.h                # OrderData / SampleData 구조체
        ├── DateUtils.h                 # 날짜 변환 유틸리티
        ├── OrderGenerator.h/.cpp       # 주문 데이터 생성기
        └── SampleGenerator.h/.cpp     # 샘플 데이터 생성기
```

## 상태 분포 조정

`src/Config.h`의 가중치 값을 수정하여 상태 비율을 조정할 수 있습니다.  
네 값의 합은 반드시 100이어야 합니다.

```cpp
int weightPending    = 40;  // 40%
int weightProcessing = 25;  // 25%
int weightCompleted  = 25;  // 25%
int weightCancelled  = 10;  // 10%
```
