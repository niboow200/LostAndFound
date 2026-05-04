#include "GameManager.h"
#include "Console.h"
#include "LockedItem.h"
#include "FragileItem.h"
#include "ImposterCustomer.h"
#include "VipCustomer.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;
using namespace Console;

GameManager::GameManager()
    : player_(100, 50)
    , current_phase_(GamePhase::MORNING_INTAKE)
    , day_(1) {}

// ─── 메인 루프 ────────────────────────────────────────────────────────────────

void GameManager::run() {
    printTitleCard();

    cout << "  당신은 오늘부터 " << bold("분실물 보관소") << "의 관리인입니다.\n";
    cout << "  아침에 물건을 접수하고, 낮에 주인에게 올바른 물건을 돌려주세요.\n";
    cout << "  " << paint("평판이 0이 되면 폐업입니다!", A::B_RED) << "\n\n";
    player_.printStatus();
    cout << "\n  " << dim("(Enter 키를 눌러 시작)") << "\n";
    cin.get();

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

    cout << "\033[2J\033[1;1H";
    printPhaseHeader(
        "\xe2\x98\x80",  // ☀
        to_string(day_) + "일차 아침  \xe2\x94\x80  분실물 접수",
        A::B_YELLOW);

    cout << "  오늘 접수된 분실물:\n\n";

    for (size_t i = 0; i < storage_.size(); ++i) {
        cout << "    "
             << paint("[" + to_string(i + 1) + "]", A::CYAN)
             << "  " << bold(storage_[i]->getName()) << "\n";
    }

    cout << "\n  " << dim("손님들이 찾아올 시간입니다. (Enter)") << "\n";
    cin.get();
    current_phase_ = GamePhase::AFTERNOON_SERVICE;
}

void GameManager::afternoonPhase() {
    printPhaseHeader(
        "\xe2\x98\x81",  // ☁
        to_string(day_) + "일차 낮  \xe2\x94\x80  손님 응대",
        A::B_CYAN);

    for (size_t i = 0; i < customers_.size(); ++i) {
        if (storage_.empty()) {
            cout << "\n  " << paint("보관소에 남은 물건이 없습니다.", A::YELLOW) << "\n";
            break;
        }
        handleCustomer(*customers_[i],
                       static_cast<int>(i) + 1,
                       static_cast<int>(customers_.size()));
    }

    current_phase_ = GamePhase::END_OF_DAY;
}

void GameManager::endOfDayPhase() {
    cout << "\033[2J\033[1;1H";
    printPhaseHeader(
        "\xe2\x98\x85",  // ★
        to_string(day_) + "일차 마감  \xe2\x94\x80  하루 결산",
        A::B_MAGENTA);

    player_.printStatus();

    if (player_.getReputation() <= 0) {
        cout << "\n";
        hRule();
        cout << "\n  "
             << paint("평판이 바닥났습니다. 보관소가 폐업하였습니다...", A::B_RED)
             << "\n";
        hRule();
        current_phase_ = GamePhase::GAME_OVER;
        return;
    }

    cout << "\n  내일도 계속하시겠습니까?  "
         << paint("1", A::B_GREEN) << paint(": 예", A::GREEN)
         << "  /  "
         << paint("0", A::GRAY) << paint(": 퇴근", A::GRAY)
         << "  > ";
    int choice = getPlayerChoice(1);

    if (choice == 1) {
        ++day_;
        current_phase_ = GamePhase::MORNING_INTAKE;
    } else {
        cout << "\n  " << bold("오늘도 수고하셨습니다. 안녕히 가세요!") << "\n";
        current_phase_ = GamePhase::GAME_OVER;
    }
}

// ─── 손님 응대: 행동 루프 ────────────────────────────────────────────────────

void GameManager::handleCustomer(Customer& customer, int index, int total) {
    cout << "\033[2J\033[1;1H";
    thinRule();
    cout << "\n  "
         << paint("손님 " + to_string(index) + " / " + to_string(total), A::GRAY)
         << "  "
         << A::BOLD << A::B_CYAN << customer.getName() << A::RST
         << "\n";

    while (true) {
        cout << "\n  "
             << A::BOLD << A::B_CYAN << customer.getName() << A::RST
             << A::GRAY << "  \xe2\x80\x94  " << A::RST
             << A::CYAN << "\xe2\x80\x9c"
             << customer.getDialogueText()
             << "\xe2\x80\x9d" << A::RST
             << "\n\n  "
             << paint("[1]", A::B_GREEN) << " 물건 주기   "
             << paint("[2]", A::B_CYAN)  << " 물건 조사   "
             << paint("[0]", A::GRAY)    << " 돌려보내기"
             << "  > ";
        int action = getPlayerChoice(2);
        cout << "\n";

        if (action == 0) {
            cout << "  " << paint("\xe2\x9c\x98", A::B_RED)
                 << "  " << customer.getName() << "이(가) 실망하며 돌아갑니다.\n";
            customer.processResult(false, player_, /*isDismissed=*/true);
            cout << "\n  " << dim("(Enter 키를 눌러 계속)") << "\n";
            cin.get();
            break;
        }

        if (action == 1) {
            displayStorage();
            cout << "  줄 물건을 고르세요  " << paint("(0: 취소)", A::GRAY) << "  > ";
            int item_choice = getPlayerChoice(static_cast<int>(storage_.size()));
            cout << "\n";

            if (item_choice == 0) continue;

            const Item& selected = *storage_[item_choice - 1];

            if (customer.verifyItem(selected)) {
                cout << "  " << paint("\xe2\x9c\x94", A::B_GREEN)
                     << "  "
                     << A::B_GREEN << "\xe2\x80\x9c맞아요! 감사합니다!\xe2\x80\x9d" << A::RST
                     << "\n";
                customer.processResult(true, player_);
                storage_.erase(storage_.begin() + (item_choice - 1));
            } else {
                cout << "  " << paint("\xe2\x9c\x98", A::B_RED)
                     << "  "
                     << A::B_RED << "\xe2\x80\x9c이건 제 물건이 아닌데요...\xe2\x80\x9d" << A::RST
                     << "\n";
                customer.processResult(false, player_);
            }
            cout << "\n  " << dim("(Enter 키를 눌러 계속)") << "\n";
            cin.get();
            break;
        }

        if (action == 2) {
            handleExamineAction();
        }
    }

    cout << "\n";
    player_.printStatus();
    cout << "\n";
}

// ─── 물건 조사 ────────────────────────────────────────────────────────────────

void GameManager::handleExamineAction() {
    cout << "  " << paint("조사할 물건을 고르세요", A::B_CYAN)
         << "  " << paint("(0: 취소)", A::GRAY) << "\n\n";

    displayStorage();

    cout << "  > ";
    int choice = getPlayerChoice(static_cast<int>(storage_.size()));
    cout << "\n";

    if (choice == 0) return;

    Item& item = *storage_[choice - 1];

    thinRule();
    cout << "  "
         << paint("[ 조사 ]", A::B_CYAN) << "  "
         << bold(item.getName()) << "\n\n";

    item.examine();

    cout << "\n";
    thinRule();
    cout << "\n  " << dim("(Enter 키를 눌러 계속)") << "\n";
    cin.get();
    cout << "\033[2J\033[1;1H";
}

// ─── 일차별 데이터 로딩 ───────────────────────────────────────────────────────

void GameManager::loadDayData() {
    storage_.clear();
    customers_.clear();

    if (day_ == 1) {
        storage_.push_back(make_unique<Item>(
            1, "낡은 검정 노트북",
            "키보드의 C키가 심하게 닳아있다. 배터리 칸 안쪽에 '김철수'라는 이름이 적힌 스티커가 붙어있다.",
            "김철수"));
        storage_.push_back(make_unique<Item>(
            2, "붉은색 숄더백",
            "안쪽 작은 주머니에 카페 영수증이 있다. 이름란에 '이영희'라고 적혀있다.",
            "이영희"));
        storage_.push_back(make_unique<Item>(
            3, "투명 접이식 우산",
            "손잡이에 검은 테이프가 감겨있다. 우산 케이스 안쪽에 유성 펜으로 '박민준'이라 쓰여있다.",
            "박민준"));
        storage_.push_back(make_unique<Item>(
            4, "갈색 가죽 지갑",
            "안쪽에 명함이 끼워져 있다. '최수진, 마케팅팀 대리'라고 인쇄되어 있다.",
            "최수진"));

        customers_.push_back(make_unique<Customer>(
            "이영희", "이영희", "가방 안에 지갑이 들어있어서 빨리 찾아야 해요."));
        customers_.push_back(make_unique<Customer>(
            "김철수", "김철수", "노트북 없으면 오늘 발표를 못 해요, 제발요!"));
        customers_.push_back(make_unique<Customer>(
            "박민준", "박민준", "비 올 것 같아서요. 제 우산 있나요?"));
        customers_.push_back(make_unique<Customer>(
            "최수진", "최수진", "카드가 다 들어있어서 정말 급합니다."));
    }
    else if (day_ == 2) {
        storage_.push_back(make_unique<LockedItem>(
            5, "잠긴 서류 가방",
            "잠금이 해제됐다. 가방 안쪽 라벨에 '홍길동 대표이사'라고 새겨진 명판이 있다.",
            "홍길동", "0409"));
        storage_.push_back(make_unique<FragileItem>(
            6, "섬세한 유리 공예품",
            "섬세한 유리로 만든 꽃 장식품. 안쪽 받침에 '신사임당 작'이라는 서명이 있다.",
            "신사임당", 3));
        storage_.push_back(make_unique<Item>(
            7, "두꺼운 양장본 책",
            "책 첫 페이지에 '장보고 소장'이라고 적혀있다.",
            "장보고"));
        storage_.push_back(make_unique<Item>(
            8, "붉은 줄무늬 목도리",
            "목도리 끝 부분에 흰 실로 '유관순'이라는 이름이 수놓아져 있다.",
            "유관순"));

        customers_.push_back(make_unique<VipCustomer>(
            "홍길동", "홍길동",
            "내 서류 가방이 어딨소? 아참, 0409가 내 창업기념일이라 비밀번호도 그걸로 쓴다오."));
        customers_.push_back(make_unique<ImposterCustomer>(
            "장보구", "장보구",
            "저... 저는 장보굽니다. 제 책을 찾으러 왔어요."));
        customers_.push_back(make_unique<Customer>(
            "신사임당", "신사임당", "제 공예품 찾으러 왔어요. 조심해서 다뤄주셨죠?"));
        customers_.push_back(make_unique<Customer>(
            "유관순", "유관순", "목도리 찾으러 왔어요. 할머니 생신 선물이거든요."));
        customers_.push_back(make_unique<Customer>(
            "장보고", "장보고", "제 소장본 책이 여기 있다고 들었는데요..."));
    }
    else {
        cout << "\n  " << paint("더 많은 이야기가 준비 중입니다.", A::B_YELLOW) << "\n";
        current_phase_ = GamePhase::GAME_OVER;
    }
}

// ─── UI 헬퍼 ─────────────────────────────────────────────────────────────────

void GameManager::displayStorage() const {
    cout << "  " << A::GRAY << "현재 보관소" << A::RST << "\n";
    if (storage_.empty()) {
        cout << "    " << dim("(비어 있음)") << "\n\n";
        return;
    }
    for (size_t i = 0; i < storage_.size(); ++i) {
        cout << "    "
             << paint("[" + to_string(i + 1) + "]", A::CYAN)
             << "  " << storage_[i]->getName() << "\n";
    }
    cout << "\n";
}

int GameManager::getPlayerChoice(int max_index) const {
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail() || choice < 0 || choice > max_index) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  " << paint("0~" + to_string(max_index) + " 사이를 입력하세요", A::YELLOW) << "  > ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
    }
}
