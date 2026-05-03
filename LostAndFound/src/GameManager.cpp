#include "GameManager.h"
#include "Console.h"
#include <iostream>
#include <limits>
#include <string>

using namespace Console;

GameManager::GameManager()
    : player_(100, 50)
    , current_phase_(GamePhase::MORNING_INTAKE)
    , day_(1) {}

// ─── 메인 루프 ────────────────────────────────────────────────────────────────

void GameManager::run() {
    printTitleCard();

    std::cout << "  당신은 오늘부터 " << bold("분실물 보관소") << "의 관리인입니다.\n";
    std::cout << "  아침에 물건을 접수하고, 낮에 주인에게 올바른 물건을 돌려주세요.\n";
    std::cout << "  " << paint("평판이 0이 되면 폐업입니다!", A::B_RED) << "\n\n";
    player_.printStatus();
    std::cout << "\n  " << dim("(Enter 키를 눌러 시작)") << "\n";
    std::cin.get();

    while (current_phase_ != GamePhase::GAME_OVER) {
        switch (current_phase_) {
        case GamePhase::MORNING_INTAKE:    morningPhase();   break;
        case GamePhase::AFTERNOON_SERVICE: afternoonPhase(); break;
        case GamePhase::END_OF_DAY:        endOfDayPhase();  break;
        default: break;
        }
    }
}

// ─── 페이즈 ──────────────────────────────────────────────────────────────────

void GameManager::morningPhase() {
    loadDayData();
    if (current_phase_ == GamePhase::GAME_OVER) return;

    printPhaseHeader(
        "\xe2\x98\x80",  // ☀
        std::to_string(day_) + "일차 아침  \xe2\x94\x80  분실물 접수",  // ─
        A::B_YELLOW);

    std::cout << "  오늘 접수된 분실물:\n\n";

    for (size_t i = 0; i < storage_.size(); ++i) {
        std::cout << "    "
                  << paint("[" + std::to_string(i + 1) + "]", A::CYAN)
                  << "  " << bold(storage_[i].getName());
        const auto& desc = storage_[i].getDescription();
        if (!desc.empty())
            std::cout << "  " << dim("(" + desc + ")");
        std::cout << "\n";
    }

    std::cout << "\n  " << dim("손님들이 찾아올 시간입니다. (Enter)") << "\n";
    std::cin.get();
    current_phase_ = GamePhase::AFTERNOON_SERVICE;
}

void GameManager::afternoonPhase() {
    printPhaseHeader(
        "\xe2\x98\x81",  // ☁
        std::to_string(day_) + "일차 낮  \xe2\x94\x80  손님 응대",
        A::B_CYAN);

    for (size_t i = 0; i < customers_.size(); ++i) {
        if (storage_.empty()) {
            std::cout << "\n  " << paint("보관소에 남은 물건이 없습니다.", A::YELLOW) << "\n";
            break;
        }
        handleCustomer(customers_[i],
                       static_cast<int>(i) + 1,
                       static_cast<int>(customers_.size()));
    }

    current_phase_ = GamePhase::END_OF_DAY;
}

void GameManager::endOfDayPhase() {
    printPhaseHeader(
        "\xe2\x98\x85",  // ★
        std::to_string(day_) + "일차 마감  \xe2\x94\x80  하루 결산",
        A::B_MAGENTA);

    player_.printStatus();

    if (player_.getReputation() <= 0) {
        std::cout << "\n";
        hRule();
        std::cout << "\n  "
                  << paint("평판이 바닥났습니다. 보관소가 폐업하였습니다...", A::B_RED)
                  << "\n";
        hRule();
        current_phase_ = GamePhase::GAME_OVER;
        return;
    }

    std::cout << "\n  내일도 계속하시겠습니까?  "
              << paint("1", A::B_GREEN) << paint(": 예", A::GREEN)
              << "  /  "
              << paint("0", A::GRAY) << paint(": 퇴근", A::GRAY)
              << "  > ";
    int choice = getPlayerChoice(1);

    if (choice == 1) {
        ++day_;
        current_phase_ = GamePhase::MORNING_INTAKE;
    } else {
        std::cout << "\n  " << bold("오늘도 수고하셨습니다. 안녕히 가세요!") << "\n";
        current_phase_ = GamePhase::GAME_OVER;
    }
}

// ─── 손님 한 명 응대 ──────────────────────────────────────────────────────────

void GameManager::handleCustomer(const Customer& customer, int index, int total) {
    std::cout << "\n";
    thinRule();
    std::cout << "\n  "
              << paint("손님 " + std::to_string(index) + " / " + std::to_string(total), A::GRAY)
              << "  "
              << A::BOLD << A::B_CYAN << customer.getName() << A::RST
              << "\n\n";

    std::cout << "  "
              << A::CYAN << "\xe2\x80\x9c"  // 여는 따옴표 "
              << customer.getDialogueText()
              << "\xe2\x80\x9d" << A::RST   // 닫는 따옴표 "
              << "\n\n";

    // [확장 예정] 행동 선택지: 물건 건네기 / 질문하기 / 조사하기
    // displayActionMenu();

    displayStorage();

    std::cout << "  어떤 물건을 드릴까요?  "
              << paint("[0] 돌려보내기", A::GRAY) << "  > ";
    int choice = getPlayerChoice(static_cast<int>(storage_.size()));
    std::cout << "\n";

    if (choice == 0) {
        std::cout << "  " << paint("\xe2\x9c\x98", A::B_RED)  // ✘
                  << "  " << customer.getName() << "이(가) 실망하며 돌아갑니다.\n";
        player_.applyPenalty(0, 10);
        std::cout << "  " << paint("   \xe2\x88\x92평판 10", A::RED) << "\n";  // −
    } else {
        const Item& selected = storage_[choice - 1];
        if (customer.verifyItem(selected)) {
            std::cout << "  " << paint("\xe2\x9c\x94", A::B_GREEN)  // ✔
                      << "  "
                      << A::B_GREEN << "\xe2\x80\x9c맞아요! 감사합니다!\xe2\x80\x9d" << A::RST
                      << "\n";
            player_.applyReward(50, 5);
            std::cout << "  " << paint("   +50원  +평판 5", A::B_GREEN) << "\n";
            storage_.erase(storage_.begin() + (choice - 1));
        } else {
            std::cout << "  " << paint("\xe2\x9c\x98", A::B_RED)  // ✘
                      << "  "
                      << A::B_RED << "\xe2\x80\x9c이건 제 물건이 아닌데요...\xe2\x80\x9d" << A::RST
                      << "\n";
            player_.applyPenalty(0, 15);
            std::cout << "  " << paint("   \xe2\x88\x92평판 15", A::RED) << "\n";
        }
    }

    std::cout << "\n";
    player_.printStatus();
    std::cout << "\n";
}

// ─── 일차별 데이터 로딩 ───────────────────────────────────────────────────────

void GameManager::loadDayData() {
    storage_.clear();
    customers_.clear();

    if (day_ == 1) {
        storage_ = {
            {1, "김철수의 노트북", "오래된 검정색 노트북"},
            {2, "이영희의 가방",   "붉은색 숄더백"},
            {3, "박민준의 우산",   "투명한 접이식 우산"},
            {4, "최수진의 지갑",   "갈색 가죽 지갑"},
        };
        customers_ = {
            {"이영희", "이영희의 가방",   "가방 안에 지갑이 들어있어서 빨리 찾아야 해요."},
            {"김철수", "김철수의 노트북", "노트북 없으면 오늘 발표를 못 해요, 제발요!"},
            {"박민준", "박민준의 우산",   "비 올 것 같아서요. 제 우산 있나요?"},
            {"최수진", "최수진의 지갑",   "카드가 다 들어있어서 정말 급합니다."},
        };
    }
    else if (day_ == 2) {
        storage_ = {
            {5, "홍길동의 열쇠",   "차키와 집 열쇠 한 묶음"},
            {6, "신사임당의 코트", "고급 캐시미어 코트"},
            {7, "장보고의 책",     "『바다의 역사』 두꺼운 양장본"},
            {8, "유관순의 목도리", "붉은 줄무늬 목도리"},
        };
        customers_ = {
            {"신사임당", "신사임당의 코트",  "어머, 여기 맡겼었는데 맞죠?"},
            {"홍길동",   "홍길동의 열쇠",    "열쇠 없으면 차도 집도 못 들어가요."},
            {"유관순",   "유관순의 목도리",  "할머니 생신 선물로 받은 거라 소중해요."},
            {"장보고",   "장보고의 책",      "도서관 반납 기한이 내일이에요!"},
        };
    }
    else {
        // [확장 예정] day_ >= 3: 파일 로딩 또는 절차적 생성
        std::cout << "\n  " << paint("더 많은 이야기가 준비 중입니다.", A::B_YELLOW) << "\n";
        current_phase_ = GamePhase::GAME_OVER;
    }
}

// ─── UI 헬퍼 ─────────────────────────────────────────────────────────────────

void GameManager::displayStorage() const {
    std::cout << "  " << A::GRAY << "현재 보관소" << A::RST << "\n";
    if (storage_.empty()) {
        std::cout << "    " << dim("(비어 있음)") << "\n\n";
        return;
    }
    for (size_t i = 0; i < storage_.size(); ++i) {
        std::cout << "    "
                  << paint("[" + std::to_string(i + 1) + "]", A::CYAN)
                  << "  " << storage_[i].getName() << "\n";
    }
    std::cout << "\n";
}

int GameManager::getPlayerChoice(int max_index) const {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < 0 || choice > max_index) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  " << paint("0~" + std::to_string(max_index) + " 사이를 입력하세요", A::YELLOW) << "  > ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}
