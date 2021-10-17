#define _CRT_SECURE_NO_WARNINGS // additional line for Visual Studio

#include <bangtal>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace bangtal;

// 게임 메인 화면
ScenePtr background;

/**
*  게임에서 사용할 아이템과 점수판을 만드는 함수
**/
ObjectPtr chat, mic, micStatus, note, coffee, coffeeHeart[3], snack, snackHeart[5],
phone, attitude, attitudeScore[6], study, studyScore[6],
wake;
SoundPtr soundLecture, soundGPA, soundCoffee, soundSnack, soundPhone;
void settingGame() {
	// 아이템 오브젝트 생성
	chat = Object::create("images/item/chat.png", background, 100, 600);	  // 채팅
	mic = Object::create("images/item/mic.png", background, 100, 450);		  // 마이크
	micStatus = Object::create("images/item/OFF.png", background, 100, 420);  // 마이크 상태(ON/OFF) 이미지
	note = Object::create("images/item/note.png", background, 100, 300);	  // 필기
	coffee = Object::create("images/item/coffee.png", background, 1080, 600); // 커피
	for (int i = 0; i < 3; i++)												  // 커피 남은 횟수 이미지
		coffeeHeart[i] = Object::create("images/item/heart.png", background, 1080 + i * 20, 580);
	snack = Object::create("images/item/snack.png", background, 1080, 450);   // 과자
	for (int i = 0; i < 5; i++)												  // 과자 남은 횟수 이미지
		snackHeart[i] = Object::create("images/item/heart.png", background, 1080 + i * 20, 430);
	phone = Object::create("images/item/phone.png", background, 1080, 300);   // 휴대폰

	// 점수 오브젝트 생성
	attitude = Object::create("images/life/태도.png", background, 350, 675);  // 태도
	for (int i = 0; i < 6; i++)												  // 태도점수 이미지
		attitudeScore[i] = Object::create("images/life/태도점수.png", background, 420 + i * 83, 675);
	study = Object::create("images/life/성적.png", background, 350, 625);	  // 성적
	for (int i = 0; i < 6; i++)												  // 성적점수 이미지
		studyScore[i] = Object::create("images/life/성적점수.png", background, 420 + i * 83, 625);

	// 깨우기 버튼 오브젝트 생성
	wake = Object::create("images/item/wake.png", background, 550, 50);		  // 깨우기 버튼
	wake->hide();															  // 조는 상태에만 버튼 보이도록 숨김

	// 사운드 오브젝트 생성
	soundLecture = Sound::create("sound/반야심경.mp3");
	soundGPA = Sound::create("sound/반야심경 리믹스.mp3");
	soundCoffee = Sound::create("sound/커피.mp3");
	soundSnack = Sound::create("sound/과자.mp3");
	soundPhone = Sound::create("sound/휴대폰.mp3");
}

/**
*  게임 시작 시  상태를 설정하는 함수
**/
bool isMicOn, isSleeping;
int eventLimit, coffeeLimit, snackLimit, attitudeLevel, studyLevel;
void settingItem() {
	background->setImage("images/background.jpg");
	isMicOn = false;														  // 마이크 상태
	isSleeping = false;														  // 조는 상태
	eventLimit = 20;														  // 게임 내 이벤트 발생 횟수
	coffeeLimit = 3;														  // 커피 남은 횟수
	snackLimit = 5;															  // 과자 남은 횟수
	attitudeLevel = 6;														  // 태도점수
	studyLevel = 6;															  // 성적점수
}

// 게임 처음 진입시 실행되는 함수
void intoGame() {
	settingGame();
	settingItem();
}

// 아이템 숨기기
void hideItem() {
	chat->hide();
	mic->hide();
	micStatus->hide();
	note->hide();
	coffee->hide();
	for (int i = coffeeLimit; i > 0; i--)
		coffeeHeart[i - 1]->hide();
	snack->hide();
	for (int i = snackLimit; i > 0; i--)
		snackHeart[i - 1]->hide();
	phone->hide();
	wake->hide();
	attitude->hide();
	for (int i = attitudeLevel; i > 0; i--)
		attitudeScore[i - 1]->hide();
	study->hide();
	for (int i = studyLevel; i > 0; i--)
		studyScore[i - 1]->hide();
}

// 아이템 보이기
void showItem() {
	chat->show();
	mic->show();
	micStatus->show();
	note->show();
	coffee->show();
	for (int i = 0; i < 2; i++)
		coffeeHeart[i]->show();
	snack->show();
	for (int i = 0; i < 5; i++)
		snackHeart[i]->show();
	phone->show();
	wake->show();
	attitude->show();
	for (int i = 0; i < 6; i++)
		attitudeScore[i]->show();
	study->show();
	for (int i = 0; i < 6; i ++)
		studyScore[i]->show();
}

// 발생한 이벤트에 선택된 아이템이 적절한지 판단하기 위한 변수
int eventCode, itemCode;

// 태도 점수 감점
int a;
void attitudeDecrease() {
	// 1에서 3점까지 랜덤하게 감점
	while (a == 0)
		a = rand() % 4;
	for (a; (a > 0) && (attitudeLevel > 0); a--) {
		attitudeScore[attitudeLevel - 1]->hide();
		attitudeLevel--;
	}
}

// 태도 점수 부여
void attitudeIncrease() {
	int a = 1;
	if (itemCode == 5) // coffee
		a = 2;
	attitudeLevel = min(6, attitudeLevel + a);
	for (int i = 0; i < attitudeLevel; i++)
		attitudeScore[i]->show();
}

// 성적 점수 감점
void studyDecrease() {
	// 1에서 3점까지 랜덤하게 감점
	while (a == 0)
		a = rand() % 4;
	for (a; (a > 0) && (studyLevel > 0); a--) {
		studyScore[studyLevel - 1]->hide();
		studyLevel--;
	}
}

// 성적 점수 부여
void studyIncrease() {
	studyLevel = min(6, studyLevel + 1);
	for (int i = 0; i < studyLevel; i++)
		studyScore[i]->show();
}

// 이벤트 처리 시간 제한
TimerPtr timer;

// 이벤트 시간 제한 초기화
void resetTiemr() {
	timer->stop();
	timer->set(2);
}

// 게임 내에서 발생하는 이벤트들
void eventChat() {
	eventCode = 1;
	background->setImage("images/채팅출석.jpg");
	timer->start();
}

void eventMic() {
	eventCode = 2;
	background->setImage("images/마이크출석.jpg");
	timer->start();
}

void eventNote() {
	eventCode = 3;
	background->setImage("images/필기.jpg");
	timer->start();
}

void eventSleep() {
	eventCode = 4;
	background->setImage("images/background.jpg");
	background->setLight(0.2);
	isSleeping = true;
	wake->show();
	timer->start();
}

/**
*  이벤트를 랜덤으로 발생시키는 함수
**/
int event = 0;
void randomEvent() {
	srand((unsigned int)time(NULL));

	int buff = event;
	while (event == buff)
		event = rand() % 4;

	if (event == 0) {
		eventChat();
	}
	else if (event == 1) {
		eventMic();
	}
	else if (event == 2) {
		eventNote();
	}
	else {
		eventSleep();
	}
}

/**
*  최종 성적을 보여주는 함수
**/
ObjectPtr attitudeGPA, studyGPA, finalGPA, restart;
void GPA() {
	soundLecture->stop();
	soundGPA->play();

	// 성적 발표 화면 보이기
	background->setLight(1);
	background->setImage("images/학점.jpg");

	// 아이템 숨기기
	hideItem();

	// 점수 번호로 표시, sprintf로 이미지 패스 부여
	// 변경될 이미지 파일의 경로 저장
	char path[40];

	sprintf(path, "images/grade/%d.png", attitudeLevel);
	attitudeGPA = Object::create(path, background, 680, 320);
	sprintf(path, "images/grade/%d.png", studyLevel);
	studyGPA = Object::create(path, background, 680, 220);

	// 점수에 따라 GPA 부여
	sprintf(path, "images/grade/GPA/%d.png", attitudeLevel + studyLevel);
	finalGPA = Object::create(path, background, 800, 150);

	// 다시 하기 버튼
	restart = Object::create("images/다시하기.png", background, 600, 100);
	restart->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		// 성적 숨기기
		attitudeGPA->hide();
		studyGPA->hide();
		finalGPA->hide();
		// 아이템 상태 게임 시작 상태로 초기화
		settingItem();
		// 아이템 보이기
		showItem();
		// 다시보기 버튼 숨기기
		restart->hide();
		// 첫 이벤트 발생, 게임 시작
		randomEvent();
		soundGPA->stop();
		soundLecture->play();

		return true;
		});
}

/**
*  랜덤 이벤트 발생이나 최종 성적 부여를 결정하는 함수
**/
void makeEvent() {
	// 정해진 숫자의 랜덤 이벤트 발생
	if (eventLimit > 0) {
		eventLimit--;
		randomEvent();
	}
	// 성적 발표
	else
		GPA();
}

/**
* 게임 시작 화면과 게임 방법을 보이는 함수
**/
ObjectPtr script;
int scriptNumber, scriptMaximum;
void showScript() {
	// 게임 시작 화면
	script = Object::create("images/1.jpg", background, 0, 0);

	// 변경될 이미지 파일의 경로 저장
	char path[20];

	// 게임 방법 설명 화면으로 이미지 변경
	scriptNumber = 2;
	scriptMaximum = 8;
	script->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		// 게임 방법 순서대로 보이기
		if (scriptNumber <= scriptMaximum) {
			sprintf(path, "images/%d.jpg", scriptNumber++);
			script->setImage(path);
		}
		else {
			// 게임 방법 숨기기
			script->hide();
			// 첫 이벤트 발생, 게임 시작
			makeEvent();
			// 배경음악
			soundLecture->play();
		}
		return true;
		});
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	background = Scene::create("", "images/background.jpg");

	// 이벤트 처리 시간 제한
	timer = Timer::create(3);

	// 게임 시작 전 게임 세팅
	intoGame();

	// 게임 시작 화면과 게임 설명
	showScript();

	// 이하 게임 아이템 동작
	// 채팅
	chat->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		// 이벤트 시간 제한 초기화
		resetTiemr();

		// 부적절하게 선택하면 태도 점수 감점
		itemCode = 1;
		if (eventCode != itemCode)
			attitudeDecrease();

		// 랜덤 이벤트 혹은 학점 부여
		makeEvent();

		return true;
		});

	// 마이크
	mic->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		// 이벤트 시간 제한 초기화
		resetTiemr();

		// 기존의 ON/OFF 상태에서 반전
		isMicOn = !(isMicOn);
		if (isMicOn)
			micStatus->setImage("images/item/ON.png");
		else
			micStatus->setImage("images/item/OFF.png");

		// 부적절하게 마이크 키면 태도 점수 감점
		// 마이크 끄는 것은 감점이나 이벤트 발생 시키지 않음
		itemCode = 2;
		if (isMicOn) {
			if (eventCode != itemCode)
				attitudeDecrease();

			// 랜덤 이벤트 혹은 학점 부여
			makeEvent();
		}
		return true;
		});

	// 필기
	note->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		// 이벤트 시간 제한 초기화
		resetTiemr();

		// 부적절하게 선택하면 성적 점수 감점
		itemCode = 3;
		if (eventCode != itemCode)
			studyDecrease();

		// 적절하게 선택하면 성적 점수 부여
		else
			studyIncrease();

		// 랜덤 이벤트 혹은 학점 부여
		makeEvent();

		return true;
		});

	// 깨우기
	wake->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		// 이벤트 시간 제한 연장
		timer->set(3);

		// 랜덤한 확률로 마이크 켜짐
		event = rand() % 2;
		if (event == 1) {
			isMicOn = true;
			micStatus->setImage("images/item/ON.png");
		}
		// 깨우면
		// 조는 상태 -> 깨어있는 상태
		// 깨우기 버튼 숨기기
		// 배경 조명 많이 어둡게 하기->조금 어둡게 하기
		isSleeping = false;
		wake->hide();
		background->setLight(0.5);
		return true;
		});

	// 커피
	coffee->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		soundCoffee->play();

		// 이벤트 시간 제한 초기화
		resetTiemr();

		// 커피 사용할 수 있는 경우
		// 푸앙이를 깨웠고 사용 횟수가 남아있는 경우
		if (!(isSleeping) && (coffeeLimit > 0)) {
			background->setLight(1.0);
			itemCode = 5;

			// 마이크 켜진 상태에서 마시면 태도점수 감점
			if (isMicOn)
				attitudeDecrease();

			// 마이크 꺼진 상태에서 마시면 태도 점수 + 2
			else
				attitudeIncrease();

			// 커피 횟수 차감
			if (coffeeLimit > 0) {
				coffeeHeart[coffeeLimit - 1]->hide();
				coffeeLimit--;
			}

			// 랜덤 이벤트 혹은 학점 부여
			makeEvent();
		}
		// 커피 사용할 수 없는 경우
		// 아무 것도 하지 않음
		else return true;

		return true;
		});

	// 과자
	snack->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		soundSnack->play();

		// 이벤트 시간 제한 초기화
		resetTiemr();

		// 커피 사용할 수 있는 경우
		if (!(isSleeping) && (snackLimit > 0)) {
			background->setLight(1.0);
			itemCode = 6;

			// 마이크 켜진 상태에서 먹으면 태도점수 감점
			if (isMicOn)
				attitudeDecrease();

			// 마이크 꺼진 상태에서 먹으면 태도 점수 +1
			else
				attitudeIncrease();

			// 과자 횟수 차감
			if (snackLimit > 0) {
				snackHeart[snackLimit - 1]->hide();
				snackLimit--;
			}

			// 랜덤 이벤트 혹은 학점 부여
			makeEvent();
		}
		// 과자 사용할 수 없는 경우
		// 아무 것도 하지 않음
		else return true;

		return true;
		});

	// 휴대폰
	phone->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		soundPhone->play();

		// 이벤트 시간 제한 초기화
		resetTiemr();

		// 휴대폰 사용할 수 있는 경우
		if (!(isSleeping)) {
			background->setLight(1.0);
			itemCode = 7;

			// 마이크 켜진 상태에서 사용하면 태도 점수 감점
			if (isMicOn)
				attitudeDecrease();

			// 마이크 꺼진 상태에서 사용하면 태도 점수 +1, 학습 점수 -1
			else {
				attitudeIncrease();
				studyDecrease();
			}

			// 랜덤 이벤트 혹은 학점 부여
			makeEvent();
		}
		// 휴대폰 사용할 수 없는 경우
		// 아무 것도 하지 않음
		else return true;

		return true;
		});

	// 이벤트 제한 시간 내에 처리하지 못한 경우
	timer->setOnTimerCallback([&](TimerPtr timer)->bool {
		// 채팅 이벤트 처리 실패시 태도 점수 감점
		if ((eventCode == 1) || (eventCode == 2)) {
			attitudeDecrease();
		}
		// 필기 이벤트 처리 실패시 성적 점수 감점
		if (eventCode == 3) {
			studyDecrease();
		}
		// 졸음 이벤트 처리 실패기 태도와 성적 점수 감점
		if (eventCode == 4) {
			attitudeDecrease();
			studyDecrease();
		}
		// 타이머 초기화
		resetTiemr();

		// 랜덤 이벤트 혹은 학점 부여
		makeEvent();

		return true;
		});

	startGame(background);
}