#include<SFML/Graphics.hpp>
#include<iostream>
#include<SFML/Audio.hpp>
#include <SFML/System.hpp>
#include<stack>
#include<deque>
#include<sstream>
#include<fstream>


using namespace std;

const int MAXN = 50;

#pragma region INIT
typedef pair <int, int> ii;
typedef pair<int, ii> iii;
//Variables
bool B1[MAXN][MAXN], B2[MAXN][MAXN], B[3][MAXN][MAXN];
bool play = true;
int screenWidth = 800, screenHeight = 600;
int alpha = 0;
bool turnOffSplash = false;
int mouseX = 0, mouseY = 0;
int n = 0;
bool enter = false, enteragain = false, chooseSizeSucceed = false;
bool pcstate = false, drawableInit = false;
int boxSize = 0, board = 0;
int X, Y, i, j, u = 0, v = 0;
bool player1Turn = true, player2Turn = false;
bool undostate = false;
int winer = 0;
int gameNameWidth = 248, gameNameHeight = 67;
int ppModeWidth = 245;
bool playMusic;
bool resume = false;
int playerIcon = 0;
bool chooseIconSucceed = false;
bool playerTurn = true;
int level = 0;
int turn = 0;
long long attack[7] = { 0,3,27,99,729,6561,59049 };
long long defense[7] = { 0,9,54,162,1458,13112,118008};
int winX, winY;
bool enterTextPlayer1 = false, enterTextPlayer2 = false, enterTextPlayer = false;
int xScore = 0, oScore = 0;
bool still = false; //continue play
bool enterNameFile = false;
bool enterNameSucceed = false;
bool loadError = false;
bool chooseModeToLoad,loadPC;
int GAMES = 0;
bool chooseNumberSucceed = false;
iii D[MAXN * MAXN];
stack <iii> S, Save;
enum GameState {
	SPLASH, MENU, PLAY, END, CHOOSE_SIZE, CHOOSEMODE, STATISTIC, SAVE, WIN,LOAD,VIEW,ABOUTUS
};
GameState state = SPLASH;

//Texture
sf::Texture logoText;
sf::Texture gameNameText, ppModeText, pcModeText, pc1Text, pc2Text, pc3Text, resumeGameText, statisticText;
sf::Texture chooseSizeText, numberBoxText, numberBoxText2, chooseIconText, xBoxText, oBoxText, startText;
sf::Texture soundOnText, soundOffText, undoText, backText, bigXBoxText, bigOBoxText, emptyText;
sf::Texture winText, continueplayText, exitText;
sf::Texture smallXText, smallOText;
sf::Texture blackXText, blackOText;
sf::Texture saveText, loadText,aboutUsText;
sf::Texture scoreText;
sf::Texture saveButtonText;
sf::Texture loadButtonText;

//Sprites

sf::Sprite gameName, ppMode, pcMode, pc1, pc2, pc3, resumeGame, statistics;
sf::Sprite chooseSize, numberBox, numberBox2, chooseIcon, xBox, oBox, start;
sf::Sprite sound, undo, back;
sf::Sprite win, continuePlay;
sf::Sprite exitBox;
sf::Sprite save, load,aboutUs;
sf::Sprite scoreBoard;
sf::Sprite saveButton;
sf::Sprite loadButton;

//Shape
sf::RectangleShape gameBoard, information, box[MAXN][MAXN];
sf::RectangleShape boxName1, boxName2;

//Fonts

sf::Font font;

//Text
sf::Text player1, player2, rule1, rule2, rule3;
sf::String playInput;
sf::Text playText, noti;
sf::Text winNoti;
sf::Text player, computer;
sf::Text score;
sf::Text asking, name,nameNoti;
sf::Text pvc, numberOfGame, xWin, oWin, draw, enterNumber;

//Sound
sf::SoundBuffer buffer;
sf::Sound click;
sf::Music music;



void InitCont() {
	while (!S.empty())
	{
		S.pop();
	}
	while (!Save.empty()) {
		Save.pop();
	}
	memset(B1, true, sizeof(B1));
	memset(B2, true, sizeof(B2));
	memset(B, true, sizeof(B));

	//init Variable

	turn = 0;
	playerTurn = true;
	enter = false, enteragain = false, chooseSizeSucceed = false;
	winer = 0;
	playInput = "";
	player1Turn = true; player2Turn = false;
	undostate = false;
	u = 0; v = 0;
	boxSize = 0; board = 0;
	playMusic = false;
	resume = false;
	pcstate = false;
	chooseIconSucceed = false;
	winX = 0; winY = 0;
}

void InitForSaveAndLoad() {
	loadPC = false;
	chooseModeToLoad = false;
	enterNameFile = true;
	enterNameSucceed = false;
	loadError = false;
	nameNoti.setCharacterSize(30);
	nameNoti.setFont(font);
	nameNoti.setFillColor(sf::Color::White);
	nameNoti.setString("");
	nameNoti.setPosition((screenWidth - nameNoti.getGlobalBounds().width) / 2, 265);

	asking.setCharacterSize(30);
	asking.setFont(font);
	asking.setFillColor(sf::Color::White);
	asking.setString("Enter a name to save:");
	asking.setPosition((screenWidth - asking.getGlobalBounds().width) / 2, 144);

	name.setCharacterSize(30);
	name.setFont(font);
	name.setFillColor(sf::Color::Black);
	name.setString("| ");
	name.setPosition((screenWidth - name.getGlobalBounds().width) / 2, 220);

	boxName1.setSize(sf::Vector2f(300, 40));
	boxName1.setFillColor(sf::Color::Black);
	boxName1.setOutlineThickness(1.0f);
	boxName1.setOutlineColor(sf::Color::White);
	boxName1.setPosition((screenWidth - 300) / 2, 216);

	boxName2.setSize(sf::Vector2f(300, 40));
	boxName2.setFillColor(sf::Color::White);
	boxName2.setOutlineThickness(1.0f);
	boxName2.setOutlineColor(sf::Color::White);
	boxName2.setPosition((screenWidth - 300) / 2, 216);

	saveButton.setPosition((screenWidth - saveButton.getGlobalBounds().width) / 2, 550);
	saveButton.setTexture(saveButtonText);

	loadButton.setTexture(loadButtonText);
	loadButton.setPosition((screenWidth - loadButton.getGlobalBounds().width) / 2, 550);
}

void InitForStatistic(int x) {
	chooseNumberSucceed = false;
	int countGame, numberOfWinX, numberOfWinO, numberOfDraw;
	if (x == 0) {
		ifstream fileIn("Data/Statistics/PVP/summary.txt");
		fileIn >> countGame;
		fileIn >> numberOfWinX;
		fileIn >> numberOfWinO;
		fileIn >> numberOfDraw;
		GAMES = countGame;
	}
	else {
		ifstream fileIn("Data/Statistics/PVC/summary.txt");
		fileIn >> countGame;
		fileIn >> numberOfWinX;
		fileIn >> numberOfWinO;
		fileIn >> numberOfDraw;
		GAMES = countGame;
	}
	stringstream str;

	str << countGame;
	numberOfGame.setFont(font);
	numberOfGame.setFillColor(sf::Color::White);
	numberOfGame.setCharacterSize(30);
	numberOfGame.setString("Games: " + str.str());
	numberOfGame.setPosition((screenWidth - numberOfGame.getGlobalBounds().width) / 2, 50);

	str.str("");
	str << numberOfWinX;
	xWin.setFont(font);
	xWin.setFillColor(sf::Color::White);
	xWin.setCharacterSize(30);
	xWin.setString("X WIN: " + str.str());
	xWin.setPosition((screenWidth - xWin.getGlobalBounds().width) / 2, 130);

	str.str("");
	str << numberOfWinO;
	oWin.setFont(font);
	oWin.setFillColor(sf::Color::White);
	oWin.setCharacterSize(30);
	oWin.setString("O WIN: " + str.str() );
	oWin.setPosition((screenWidth - oWin.getGlobalBounds().width) / 2, 210);

	str.str("");
	str << numberOfDraw;
	draw.setFont(font);
	draw.setFillColor(sf::Color::White);
	draw.setCharacterSize(30);
	draw.setString("DRAW: " + str.str());
	draw.setPosition((screenWidth - draw.getGlobalBounds().width) / 2, 290);
	

	//enterNumber.setPosition((screenWidth - enterNumber.getGlobalBounds().width) / 2, 460);
	enterNumber.setFont(font);
	enterNumber.setFillColor(sf::Color::Black);
	enterNumber.setCharacterSize(35);
	enterNumber.setString("|");
	enterNumber.setPosition((screenWidth - enterNumber.getGlobalBounds().width) / 2 -1, 460);

	back.setPosition((screenWidth - back.getGlobalBounds().width), screenHeight - back.getGlobalBounds().height);

	numberBox2.setPosition((screenWidth - numberBox2.getLocalBounds().width) / 2, 460);

	loadButton.setPosition((screenWidth - loadButton.getLocalBounds().width) / 2, 540);
}

void InitForViewPVP(int num) {
	pcstate = false;
	stringstream str;
	str << "Data/Statistics/PVP/" << num << ".txt";
	ifstream fileIn(str.str().c_str());

	if (fileIn.is_open()) {
		fileIn >> n;
		cout << n << endl;
		fileIn >> xScore >> oScore;
		cout << xScore << " " << oScore << endl;

		char s[100];

		fileIn.ignore();

		fileIn.getline(s, 100);
		string r;
		r = s;
		cout << r << endl;
		player1.setString(r);

		fileIn.getline(s, 100);
		r = s;
		cout << r << endl;
		player2.setString(r);

		fileIn >> turn;
		cout << turn;
		for (int i = 1; i <= turn; i++)
		{
			fileIn >> D[i].first >> D[i].second.first >> D[i].second.second;
			int icon = D[i].first;
			int x = D[i].second.first;
			int y = D[i].second.second;
			if (icon == 1) {
				B1[x][y] = false;
				box[x][y].setTexture(&bigXBoxText);
			}
			else {
				B2[x][y] = false;
				box[x][y].setTexture(&bigOBoxText);
			}
			//S.push(make_pair(icon, make_pair(x, y)));
			cout << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
		}
		fileIn >> winX >> winY >> winer;
	}

}

void InitForViewPVC(int num) {
	pcstate = true;
	stringstream str;
	str << "Data/Statistics/PVC/" << num << ".txt";
	ifstream fileIn(str.str().c_str());

	if (fileIn.is_open()) {
		fileIn >> n;
		cout << n << endl;

		fileIn >> xScore >> oScore;
		cout << xScore << " " << oScore << endl;

		fileIn >> playerIcon;
		cout << playerIcon << endl;

		char s[100];

		fileIn.ignore();

		fileIn.getline(s, 100);
		string r;
		r = s;
		cout << r << endl;
		player.setString(r);

		fileIn >> turn;
		cout << turn;
		for (int i = 1; i <= turn; i++)
		{
			fileIn >> D[i].first >> D[i].second.first >> D[i].second.second;
			int icon = D[i].first;
			int x = D[i].second.first;
			int y = D[i].second.second;
			if (icon == 1) {
				B1[x][y] = false;
				box[x][y].setTexture(&bigXBoxText);
			}
			else {
				B2[x][y] = false;
				box[x][y].setTexture(&bigOBoxText);
			}
			//S.push(make_pair(icon, make_pair(x, y)));
			cout << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
		}
		fileIn >> winX >> winY >> winer;
	}
}

void Init() {

	while (!S.empty())
	{
		S.pop();
	}
	while (!Save.empty()) {
		Save.pop();
	}
	memset(B1, true, sizeof(B1));
	memset(B2, true, sizeof(B2));
	memset(B, true, sizeof(B));

	//init Variable

	turn = 0;
	playerTurn = true;
	enter = false, enteragain = false, chooseSizeSucceed = false;
	winer = 0;
	playInput = "";
	player1Turn = true; player2Turn = false;
	undostate = false;
	u = 0; v = 0;
	boxSize = 0; board = 0;
	playMusic = false;
	resume = false;
	pcstate = false;
	chooseIconSucceed = false;
	winX = 0; winY = 0;
	

	//gameName
	int posXName = (screenWidth - gameNameWidth) / 2, posYName = (screenHeight - gameNameHeight) / 2;
	gameName.setTexture(gameNameText);
	gameName.setPosition(posXName, 30);

	//ppMode
	int posXppMode = (screenWidth - ppModeWidth) / 2;
	ppMode.setTexture(ppModeText);
	ppMode.setPosition(posXppMode, 135);

	//pcMode
	pcMode.setTexture(pcModeText);
	pcMode.setPosition(posXppMode, 263);

	//Level 1,2,3
	pc1.setTexture(pc1Text);
	pc1.setPosition(posXppMode, 360);

	pc2.setTexture(pc2Text);
	pc2.setPosition(366, 360);

	pc3.setTexture(pc3Text);
	pc3.setPosition(454, 360);

	//others option	
	resumeGame.setTexture(resumeGameText);
	resumeGame.setPosition(posXppMode, 441);

	statistics.setTexture(statisticText);
	statistics.setPosition(posXppMode, 519);

	aboutUs.setTexture(aboutUsText);
	aboutUs.setPosition(54, 519);

	load.setTexture(loadText);
	load.setPosition(576, 519);

	//set up before start
	chooseSize.setTexture(chooseSizeText);
	chooseSize.setPosition(216, 108);

	numberBox.setTexture(numberBoxText);
	numberBox.setPosition(327, 219);
	numberBox2.setTexture(numberBoxText2);
	numberBox2.setPosition(327, 219);

	chooseIcon.setTexture(chooseIconText);
	chooseIcon.setPosition(290, 360);
	xBox.setTexture(xBoxText);
	xBox.setPosition(290, 408);
	oBox.setTexture(oBoxText);
	oBox.setPosition(451, 408);

	start.setTexture(startText);
	start.setPosition(330, 517);

	//Enter Text

	playText.setFillColor(sf::Color::Black);
	playText.setCharacterSize(40);
	playText.setFont(font);
	playText.setPosition(365, 219);
	playText.setString(playInput);
	noti.setFont(font);
	noti.setCharacterSize(20);
	noti.setPosition(252, 280);
	noti.setFillColor(sf::Color::White);

	//Play screen
	//gameboard

	gameBoard.setFillColor(sf::Color::Black);
	gameBoard.setOutlineThickness(1.0f);
	gameBoard.setOutlineColor(sf::Color::White);
	information.setFillColor(sf::Color::Black);
	information.setOutlineColor(sf::Color::White);
	information.setOutlineThickness(1.0f);
	for (i = 0; i < MAXN; i++)
		for (j = 0; j < MAXN; j++) {
			box[i][j].setTexture(&emptyText);
			box[i][j].setOutlineColor(sf::Color::Black);
			box[i][j].setOutlineThickness(1.0f);
		}

	sound.setTexture(soundOffText);
	sound.setPosition(615, 210);
	undo.setTexture(undoText);
	undo.setPosition(615, 300);
	back.setTexture(backText);
	back.setPosition(715, 210);
	save.setTexture(saveText);
	save.setPosition(715, 300);
	scoreBoard.setTexture(scoreText);
	scoreBoard.setPosition(620, 396);

	//Score

	score.setString("0 : 0");
	score.setFont(font);
	score.setCharacterSize(30);
	score.setFillColor(sf::Color::White);
	score.setPosition(600 + (200 - score.getGlobalBounds().width) / 2, 468);


	player1.setCharacterSize(20);
	player2.setCharacterSize(20);
	player1.setFont(font);
	player2.setFont(font);
	player1.setFillColor(sf::Color::White);
	player2.setFillColor(sf::Color::White);
	player1.setPosition(684, 50);
	player2.setPosition(684, 140);
	if (!still)
	{
		player1.setString("Player 1");
		player2.setString("Player 2");
	}
	player.setCharacterSize(20);
	player.setFont(font);
	player.setFillColor(sf::Color::White);
	//player.setPosition(684, 50);
	if (!still) player.setString("Player");

	computer.setCharacterSize(20);
	computer.setFont(font);
	computer.setFillColor(sf::Color::White);
	//computer.setPosition(684, 140);
	computer.setString("Computer");

	// rule 1 apply to 3x3  and 4x4
	rule1.setPosition(615, 550);
	rule1.setFont(font);
	rule1.setCharacterSize(21);
	rule1.setFillColor(sf::Color::White);
	rule1.setString("3 in a line to win.");

	//rule2 apply to 5x5 & 6x6
	rule2.setPosition(615, 550);
	rule2.setFont(font);
	rule2.setCharacterSize(21);
	rule2.setFillColor(sf::Color::White);
	rule2.setString("4 in a line to win.");

	//rule3 apply to 7x7 and more
	rule3.setPosition(615, 550);
	rule3.setFont(font);
	rule3.setCharacterSize(21);
	rule3.setFillColor(sf::Color::White);
	rule3.setString("5 in a line to win.");

	//Win

	winNoti.setCharacterSize(35);
	winNoti.setFillColor(sf::Color::White);
	winNoti.setFont(font);
	winNoti.setPosition(200, 232);

	win.setPosition(106, 182);
	win.setTexture(winText);
	continuePlay.setPosition(163, 317);
	continuePlay.setTexture(continueplayText);
	exitBox.setPosition(359, 317);
	exitBox.setTexture(exitText);

	//Audio
	music.pause();

	//SAVING
	
	
}

void loadData() {
	if (loadButtonText.loadFromFile("Data/loadbutton.png") == -1) {
		return;
	}
	if (saveButtonText.loadFromFile("Data/saveButton.png") == -1) {
		return;
	}
	if (scoreText.loadFromFile("Data/scoreboard.png") == -1) {
		return;
	}
	if (loadText.loadFromFile("Data/load.png") == -1) {
		return;
	}
	if (aboutUsText.loadFromFile("Data/aboutus.png") == -1) {
		return;
	}
	if (saveText.loadFromFile("Data/save.png") == -1) {
		return;
	}
	if (blackXText.loadFromFile("Data/blackxbox.png") == -1) {
		return;
	}
	if (blackOText.loadFromFile("Data/blackobox.png") == -1) {
		return;
	}
	if (smallXText.loadFromFile("Data/smallxbox.png") == -1) {
		return;
	}
	if (smallOText.loadFromFile("Data/smallobox.png") == -1) {
		return;
	}
	if (chooseSizeText.loadFromFile("Data/choosesize.png") == -1)
	{
		return;
	}
	if (logoText.loadFromFile("Data/logo.png") == -1) {
		return;
	}
	if (gameNameText.loadFromFile("Data/gamename.png") == -1)
	{
		return;
	}
	if (ppModeText.loadFromFile("Data/ppmode.png") == -1)
	{
		return;
	}
	if (pcModeText.loadFromFile("Data/pcmode.png") == -1)
	{
		return;
	}
	if (pc1Text.loadFromFile("Data/pc1.png") == -1)
	{
		return;
	}
	if (pc2Text.loadFromFile("Data/pc2.png") == -1)
	{
		return;
	}
	if (pc3Text.loadFromFile("Data/pc3.png") == -1)
	{
		return;
	}
	if (resumeGameText.loadFromFile("Data/resumegame.png") == -1)
	{
		return;
	}
	if (statisticText.loadFromFile("Data/statistics.png") == -1)
	{
		return;
	}
	if (numberBoxText.loadFromFile("Data/enternum.png") == -1) {
		return;
	}
	if (numberBoxText2.loadFromFile("Data/enternum1.png") == -1) {
		return;
	}
	if (chooseIconText.loadFromFile("Data/chooseicon.png") == -1) {
		return;
	}
	if (xBoxText.loadFromFile("Data/xbox.png") == -1)
	{
		return;
	}
	if (oBoxText.loadFromFile("Data/obox.png") == -1)
	{
		return;
	}
	if (startText.loadFromFile("Data/start.png") == -1) {
		return;
	}
	if (soundOnText.loadFromFile("Data/soundon.png") == -1) {
		return;
	}
	if (soundOffText.loadFromFile("Data/soundoff.png") == -1) {
		return;
	}
	if (undoText.loadFromFile("Data/undo.png") == -1) {
		return;
	}
	if (backText.loadFromFile("Data/backtomenu.png") == -1) {
		return;
	}
	if (bigXBoxText.loadFromFile("Data/bigxbox.png") == -1) {
		return;
	}
	if (bigOBoxText.loadFromFile("Data/bigobox.png") == -1) {
		return;
	}
	if (emptyText.loadFromFile("Data/empty.png") == -1) {
		return;
	}
	if (winText.loadFromFile("Data/win.png") == -1) {
		return;
	}
	if (continueplayText.loadFromFile("Data/continueplay.png") == -1) {
		return;
	}
	if (exitText.loadFromFile("Data/exit.png") == -1) {
		return;
	}
	if (buffer.loadFromFile("Data/click.wav") == 0) {
		return;
	}
	if (music.openFromFile("Data/music.ogg") == 0) {
		return;
	}

	//Font
	if (font.loadFromFile("Data/VNI-Avo.ttf") == 0) {
		cout << "Not able to load font " << endl;
		return;
	}
}
#pragma endregion

#pragma region PVP
int check(int n, int k, int u, int v) {

	int count1 = 0; int count2 = 0;
	int x1, y1, x2, y2;

	//incline left to right

	for (int i = 0; i < k; i++) {
		int x = u + i;
		int y = v + i;
		if (1 <= x && x <= n && 1 <= y && y <= n) {
			if (!B1[x][y]) count1++;
			if (!B2[x][y]) count2++;
		}
		else break;
	}
	x1 = u - 1; y1 = v - 1;
	x2 = u + k; y2 = v + k;
	if (x1 > 0 && y1 > 0 && x2 <= n && y2 <= n) {
		if (count2 == k && !B1[x1][y1] && !B1[x2][y2]) return 0;
		if (count1 == k && !B2[x1][y1] && !B2[x2][y2]) return 0;
	}
	if (count1 == k) return 1;
	if (count2 == k) return 2;

	//incline right to left
	count1 = 0; count2 = 0;
	for (int i = 0; i < k; i++) {
		int x = u + i;
		int y = v - i;
		if (1 <= x && x <= n && 1 <= y && y <= n) {
			if (!B1[x][y]) count1++;
			if (!B2[x][y]) count2++;
		}
		else break;
	}
	x1 = u - 1; y1 = v + 1;
	x2 = u + k; y2 = v - k;
	if (0 < x1 && x1 <= n && 0 < x2 && x2 <= n && 0 < y1 && y1 <= n && 0 < y2 && y2 <= n) {
		if (count2 == k && !B1[x1][y1] && !B1[x2][y2]) return 0;
		if (count1 == k && !B2[x1][y1] && !B2[x2][y2]) return 0;
	}
	if (count1 == k) return 1;
	if (count2 == k) return 2;

	//In a vertical line
	count1 = 0; count2 = 0;
	for (int i = 0; i < k; i++) {
		int x = u + i;
		int y = v;
		if (1 <= x && x <= n && 1 <= y && y <= n) {
			if (!B1[x][y]) count1++;
			if (!B2[x][y]) count2++;
		}
		else break;
	}
	x1 = u - 1; y1 = v;
	x2 = u + k; y2 = v;
	if (0 < x1 && x1 <= n && 0 < x2 && x2 <= n && 0 < y1 && y1 <= n && 0 < y2 && y2 <= n) {
		if (count2 == k && !B1[x1][y1] && !B1[x2][y2]) return 0;
		if (count1 == k && !B2[x1][y1] && !B2[x2][y2]) return 0;
	}
	if (count1 == k) return 1;
	if (count2 == k) return 2;

	//In a horizontal line 
	count1 = 0; count2 = 0;
	for (int i = 0; i < k; i++) {
		int x = u;
		int y = v + i;
		if (1 <= x && x <= n && 1 <= y && y <= n) {
			if (!B1[x][y]) count1++;
			if (!B2[x][y]) count2++;
		}
		else break;
	}
	x1 = u; y1 = v - 1;
	x2 = u; y2 = v + k;
	if (0 < x1 && x1 <= n && 0 < x2 && x2 <= n && 0 < y1 && y1 <= n && 0 < y2 && y2 <= n) {
		if (count2 == k && !B1[x1][y1] && !B1[x2][y2]) return 0;
		if (count1 == k && !B2[x1][y1] && !B2[x2][y2]) return 0;
	}
	if (count1 == k) return 1;
	if (count2 == k) return 2;
	return 0;
}
#pragma endregion

#pragma region PVC
void doIt(bool &undostate, int icon) {
	
	if (!S.empty()) {
		int u = S.top().second.first;
		int v = S.top().second.second;
		if (S.top().first == 1) {
			box[u][v].setTexture(&bigXBoxText);
		}
		else
		{
			box[u][v].setTexture(&bigOBoxText);
		}
		S.pop();
	}
	if (!S.empty()) {
		int u = S.top().second.first;
		int v = S.top().second.second;
		if (S.top().first == 1) {
			box[u][v].setTexture(&bigXBoxText);
		}
		else
		{
			box[u][v].setTexture(&bigOBoxText);
		}
		S.pop();
	}
	
	if (undostate && !Save.empty()) {
		int x = Save.top().second.first;
		int y = Save.top().second.second;
		box[x][y].setTexture(&emptyText);
		//undostate = false;
		B[1][x][y] = true;
		B[2][x][y] = true;
		Save.pop();
		turn--;
	}
	if (undostate && !Save.empty()) {
		int x = Save.top().second.first;
		int y = Save.top().second.second;
		box[x][y].setTexture(&emptyText);
		undostate = false;
		B[1][x][y] = true;
		B[2][x][y] = true;
		Save.pop();
		turn--;
	}

	if (winer > 0) {

		if (winer == icon) {
			sf::String text;
			text = player.getString() + " Win";
			winNoti.setString(text);
		}
		else
			if (winer == 3 - icon) {
				winNoti.setPosition(180, 232);
				winNoti.setString("Computer Win");
			}
			else
			{
				winNoti.setString("DRAW!");
				winNoti.setPosition(250, 232);
			}
		//cout << winer;
	}

}
#pragma endregion

#pragma region PLAY PVC
long long attackVertical(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;
	for (int i = 1; i <= k && u + i <= n; i++) {
		if (!B[3 - icon][u + i][v]) {
			countCom++;
		}
		else if (!B[icon][u + i][v]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && u + i == n) sum--;
		else break;
	}
	for (int i = 1; i <= k && u - i > 0; i++) {
		if (!B[3 - icon][u - i][v]) {
			countCom++;
		}
		else if (!B[icon][u - i][v]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && u - i == 1) sum--;
		else break;
	}
	if (level == 3 && countPlayer == 1 && countCom == k-1) return 10000000;
	if (countPlayer == 2) return 0;
	//if (level == 3) return sum + attack[countCom];
	sum -= defense[countPlayer + 1];
	sum += attack[countCom];
	return sum;
}

long long attackHorizontal(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;
	for (int i = 1; i <= k && v + i <= n; i++) {
		if (!B[3 - icon][u][v + i]) {
			countCom++;
		}
		else if (!B[icon][u][v + i]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && v + i == n) sum--;
		else break;
	}
	for (int i = 1; i <= k && v - i > 0; i++) {
		if (!B[3 - icon][u][v - i]) {
			countCom++;
		}
		else if (!B[icon][u][v - i]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && v - i == 1) sum--;
		else break;
	}
	if (level == 3 && countPlayer == 1 && countCom == k-1) return 10000000;
	if (countPlayer == 2) return 0;
	//if (level == 3) return sum + attack[countCom];
	sum -= defense[countPlayer + 1];
	sum += attack[countCom];
	return sum;
}

long long attackCross(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;

	for (int i = 1; i <= k && u + i <= n && v + i <= n; i++) {
		if (!B[3 - icon][u + i][v + i]) {
			countCom++;
		}
		else if (!B[icon][u + i][v + i]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && (u + i == n || v + i == n)) sum--;
		else break;
	}

	for (int i = 1; i <= k && u - i > 0 && v - i > 0; i++) {
		if (!B[3 - icon][u - i][v - i]) {
			countCom++;
		}
		else if (!B[icon][u - i][v - i]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && (u - i == 1 || v - i == 1)) sum--;
		else break;
	}
	if (level == 3 && countPlayer == 1 && countCom == k-1) return 10000000;
	if (countPlayer == 2) return 0;
	//if (level == 3) return sum + attack[countCom];
	sum -= defense[countPlayer + 1];
	sum += attack[countCom];
	return sum;

}

long long attackReverse(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;

	for (int i = 1; i <= k && u - i > 0 && v + i <= n; i++) {
		if (!B[3 - icon][u - i][v + i]) {
			countCom++;
		}
		else if (!B[icon][u - i][v + i]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && (u - i == 1 || v + i == n)) sum--;
		else break;
	}

	for (int i = 1; i <= k && u + i <= n && v - i > 0; i++) {
		if (!B[3 - icon][u + i][v - i]) {
			countCom++;
		}
		else if (!B[icon][u + i][v - i]) {
			countPlayer++;
			if (level == 3) sum -= 9;
			break;
		}
		//else if (level == 3 && (u + i == n || v - i == 1)) sum--;
		else break;
	}
	if (level == 3 && countPlayer == 1 && countCom == k-1) return 10000000;
	if (countPlayer == 2) return 0;
	//if (level == 3) return sum + attack[countCom];
	sum -= defense[countPlayer + 1];
	sum += attack[countCom];
	return sum;
}

long long defenseVertical(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;
	for (int i = 1; i <= k && u + i <= n; i++) {
		if (!B[3 - icon][u + i][v]) {
			countCom++;
			if (level == 3) sum--;
			break;
		}
		else if (!B[icon][u + i][v]) {
			countPlayer++;
		}
		//else if (level == 3 && u + i == n) sum--;
		else break;
	}
	for (int i = 1; i <= k && u - i > 0; i++) {
		if (!B[3 - icon][u - i][v]) {
			countCom++;
			if (level == 3) sum--;
			break;
		}
		else if (!B[icon][u - i][v]) {
			countPlayer++;
		}
		//else if (level == 3 && u - i == 1) sum--;
		else break;
	}
	if (level == 3 && countCom == 1 && countPlayer == k-2) return 50;
	if (level == 3 && countCom == 0 && countPlayer == k-2) return 100000;
	if (countCom == 2) return 0;
	sum += defense[countPlayer];
	return sum;
}

long long defenseHorizontal(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;
	for (int i = 1; i <= k && v + i <= n; i++) {
		if (!B[3 - icon][u][v + i]) {
			countCom++;
			if (level == 3) sum -- ;
			break;
		}
		else if (!B[icon][u][v + i]) {
			countPlayer++;
		}
		//else if (level == 3 && (v + i == n)) sum--;
		else break;
	}
	for (int i = 1; i <= k && v - i > 0; i++) {
		if (!B[3 - icon][u][v - i]) {
			countCom++;
			if (level == 3) sum -- ;
			break;
		}
		else if (!B[icon][u][v - i]) {
			countPlayer++;
		}
		//else if (level == 3 && v - i == 1) sum--;
		else break;
	}
	if (level == 3 && countCom == 1 && countPlayer == k-2) return 50;
	if (level == 3 && countCom == 0 && countPlayer == k-2) return 100000;
	if (countCom == 2) return 0;
	sum += defense[countPlayer];
	return sum;
}

long long defenseCross(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;

	for (int i = 1; i <= k && u + i <= n && v + i <= n; i++) {
		if (!B[3 - icon][u + i][v + i]) {
			countCom++;
			if (level == 3) sum --;
			break;
		}
		else if (!B[icon][u + i][v + i]) {
			countPlayer++;
		}
		//else if (level == 3 && (u + i == n || v + i == n)) sum--;
		else break;
	}

	for (int i = 1; i <= k && u - i > 0 && v - i > 0; i++) {
		if (!B[3 - icon][u - i][v - i]) {
			countCom++;
			if (level == 3) sum --;
			break;
		}
		else if (!B[icon][u - i][v - i]) {
			countPlayer++;
		}
		//else if (level == 3 && (u - i == 1 || v - i == 1)) sum--;
		else break;
	}
	if (level == 3 && countCom == 1 && countPlayer == k-2) return 50;
	if (level == 3 && countCom == 0 && countPlayer == k-2) return 100000;
	if (countCom == 2) return 0;
	sum += defense[countPlayer];
	return sum;
}

long long defenseReverse(int u, int v, int icon, int k) {

	long long sum = 0;
	int countCom = 0;
	int countPlayer = 0;
	for (int i = 1; i <= k && u - i > 0 && v + i <= n; i++) {
		if (!B[3 - icon][u - i][v + i]) {
			countCom++;
			if (level == 3) sum --;
			break;
		}
		else if (!B[icon][u - i][v + i]) {
			countPlayer++;
		}
		//else if (level == 3 && (u - i == 1 || v + i == n)) sum--;
		else break;
	}

	for (int i = 1; i <= k && u + i <= n && v - i > 0; i++) {
		if (!B[3 - icon][u + i][v - i]) {
			countCom++;
			if (level == 3) sum --;
			break;
		}
		else if (!B[icon][u + i][v - i]) {
			countPlayer++;
		}
		//else if (level == 3 && (u + i == n || v - i == 1)) sum--;
		else break;
	}
	if (level == 3 && countCom == 1 && countPlayer == k-2) return 9;
	if (level == 3 && countCom == 0 && countPlayer == k-2) return 100000;
	if (countCom == 2) return 0;
	sum += defense[countPlayer];
	return sum;
}

ii findPosition(int icon, int n, int k) {
	ii res = make_pair(0, 0);
	long long Max = 0;
	long long MaxSum = 0;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			if (B[1][i][j] && B[2][i][j]) {
				long long attackScore = attackVertical(i, j, icon, k) + attackHorizontal(i, j, icon, k) + attackCross(i, j, icon, k) + attackReverse(i, j, icon, k);
				long long defenseScore = defenseVertical(i, j, icon, k) + defenseHorizontal(i, j, icon, k) + defenseCross(i, j, icon, k) + defenseReverse(i, j, icon, k);
				long long temporaryScore = attackScore > defenseScore ? attackScore : defenseScore;
				long long sum = attackScore + defenseScore;
				/*if (level == 3 && Max == temporaryScore) {
					if (MaxSum < sum) {
						MaxSum = sum;
						res = make_pair(i, j);
					}
				}*/
				if (Max < temporaryScore) {
					Max = temporaryScore;
					res = make_pair(i, j);
				}
			}
	return res;
}

void playLevel(int icon, int n, int k, int level) {
	if (level == 1) {
		attack[0] = 0;
		attack[1] = 10;
		attack[2] = 100; 
		attack[3] = 1000;
		attack[4] = 10000; 
		attack[5] = 100000;
		attack[6] = 1000000;

		defense[0] = 0; 
		defense[1] = 10;
		defense[2] = 100;
		defense[3] = 1000;
		defense[4] = 10000; 
		defense[5] = 100000;
		defense[6] = 1000000;
	}
	if (level == 2) {
		attack[0] = 0;
		attack[1] = 9;
		attack[2] = 54;
		attack[3] = 162;
		attack[4] = 1458;
		attack[5] = 13112;
		attack[6] = 118008;

		defense[0] = 0;
		defense[1] = 3;
		defense[2] = 27;
		defense[3] = 81;
		defense[4] = 729;
		defense[5] = 6561;
		defense[6] = 59049;		
	}
	if (level == 3) {
		attack[0] = 0;
		attack[1] = 3;
		attack[2] = 24;
		attack[3] = 192;
		attack[4] = 1536;
		attack[5] = 12288;
		attack[6] = 98304;

		defense[0] = 0;
		defense[1] = 1;
		defense[2] = 9;
		defense[3] = 81;
		defense[4] = 729;
		defense[5] = 6561;
		defense[6] = 59049;
	}
	ii pos = findPosition(icon, n, k);
	//cout << pos.first << " " << pos.second << endl;
	S.push(make_pair(3 - icon, make_pair(pos.first, pos.second)));
	Save.push(make_pair(3 - icon, make_pair(pos.first, pos.second)));
	B[3 - icon][pos.first][pos.second] = false;
	turn++;
	D[turn] = make_pair(3 - icon, make_pair(pos.first, pos.second));
}
#pragma endregion

#pragma region WIN ANIMATION PVC

void putVetical(int u, int v, int  icon) {
	for (int i = 0; i <= 6 && u + i <= n; i++)
	{
		if (B[icon][u + i][v]) break;
		if (!B[icon][u + i][v]) {
			if (icon == 1) box[u + i][v].setTexture(&blackXText);
			else box[u + i][v].setTexture(&blackOText);
		}
	}
	for (int i = 0; i <= 6 && u - i > 0; i++)
	{
		if (B[icon][u - i][v]) break;
		if (!B[icon][u - i][v]) {
			if (icon == 1) box[u - i][v].setTexture(&blackXText);
			else box[u - i][v].setTexture(&blackOText);
		}
	}
}

void putHorizontal(int u, int v, int icon) {

	for (int i = 0; i <= 6 && v + i <= n; i++)
	{
		if (B[icon][u][v+i]) break;
		if (!B[icon][u][v+i]) {
			if (icon == 1) box[u][v + i].setTexture(&blackXText);
			else box[u][v+i].setTexture(&blackOText);
		}
	}
	for (int i = 0; i <= 6 && v - i > 0; i++)
	{
		if (B[icon][u][v-i]) break;
		if (!B[icon][u][v-i]) {
			if (icon == 1) box[u][v-i].setTexture(&blackXText);
			else box[u][v-i].setTexture(&blackOText);
		}
	}

}

void putCross(int u, int v, int icon) {

	for (int i = 0; i <= 6 && u + i <= n && v+ i <=n; i++)
	{
		if (B[icon][u + i][v+i]) break;
		if (!B[icon][u + i][v+i]) {
			if (icon == 1) box[u + i][v+i].setTexture(&blackXText);
			else box[u + i][v+i].setTexture(&blackOText);
		}
	}
	for (int i = 0; i <= 6 && u - i > 0 && v -i > 0; i++)
	{
		if (B[icon][u - i][v - i]) break;
		if (!B[icon][u - i][v-i]) {
			if (icon == 1) box[u -i][v-i].setTexture(&blackXText);
			else box[u - i][v-i].setTexture(&blackOText);
		}
	}

}

void putReverse(int u, int v, int icon) {

	for (int i = 0; i <= 6 && u - i > 0 && v + i <= n; i++)
	{
		if (B[icon][u - i][v + i]) break;
		if (!B[icon][u - i][v +i]) {
			if (icon == 1) box[u - i][v + i].setTexture(&blackXText);
			else box[u - i][v + i].setTexture(&blackOText);
		}
	}
	for (int i = 0; i <= 6 && u + i <= n && v - i > 0; i++)
	{
		if (B[icon][u + i][v - i]) break;
		if (!B[icon][u + i][v - i]) {
			if (icon == 1) box[u + i][v- i].setTexture(&blackXText);
			else box[u + i][v - i].setTexture(&blackOText);
		}
	}

}
#pragma endregion

#pragma region CHECK WIN PVC

bool checkVertical(int n, int k, int u, int v, int icon) {
	int countWin = 1;
	int count = 0;
	for (int i = 1; i <= k && u + i <= n; i++) {
		if (!B[3 - icon][u + i][v]) {
			count++;
			break;
		}
		else if (!B[icon][u + i][v]) {
			countWin++;
		}
		else break;
	}
	for (int i = 1; i <= k && u - i > 0; i++) {
		if (!B[3 - icon][u - i][v]) {
			count++;
			break;
		}
		else if (!B[icon][u - i][v]) {
			countWin++;
		}
		else break;
	}
	if (countWin >= k && count < 2) return true;
	return false;
}

bool checkHorizontal(int n, int k, int u, int v, int icon) {
	int countWin = 1;
	int count = 0;
	for (int i = 1; i <= k && v + i <= n; i++) {
		if (!B[3 - icon][u][v + i]) {
			count++;
			break;
		}
		else if (!B[icon][u][v + i]) {
			countWin++;
		}
		else break;
	}
	for (int i = 1; i <= k && v - i > 0; i++) {
		if (!B[3 - icon][u][v - i]) {
			count++;
			break;
		}
		else if (!B[icon][u][v - i]) {
			countWin++;
		}
		else break;
	}
	if (countWin >= k && count < 2) return true;
	return false;
}

bool checkCross(int n, int k, int u, int v, int icon) {
	int countWin = 1;
	int count = 0;
	for (int i = 1; i <= k && v + i <= n && u + i <= n; i++) {
		if (!B[3 - icon][u + i][v + i]) {
			count++;
			break;
		}
		else if (!B[icon][u + i][v + i]) {
			countWin++;
		}
		else break;
	}
	for (int i = 1; i <= k && v - i > 0 && u - i > 0; i++) {
		if (!B[3 - icon][u - i][v - i]) {
			count++;
			break;
		}
		else if (!B[icon][u - i][v - i]) {
			countWin++;
		}
		else break;
	}
	if (countWin >= k && count < 2) return true;
	return false;
}

bool checkReverse(int n, int k, int u, int v, int icon) {
	int countWin = 1;
	int count = 0;
	for (int i = 1; i <= k && v + i <= n && u - i > 0; i++) {
		if (!B[3 - icon][u - i][v + i]) {
			count++;
			break;
		}
		else if (!B[icon][u - i][v + i]) {
			countWin++;
		}
		else break;
	}
	for (int i = 1; i <= k && v - i > 0 && u + i <= n; i++) {
		if (!B[3 - icon][u + i][v - i]) {
			count++;
			break;
		}
		else if (!B[icon][u + i][v - i]) {
			countWin++;
		}
		else break;
	}
	if (countWin >= k && count < 2) return true;
	return false;
}

bool checkPC(int n, int k, int u, int v, int icon) {

	if (checkVertical(n, k, u, v, icon)) {
		putVetical(u, v, icon);
		return true;
	}
	if (checkHorizontal(n, k, u, v, icon)) {
		putHorizontal(u, v, icon);
		return true;
	}
	if (checkCross(n, k, u, v, icon)) {
		putCross(u, v, icon);
		return true;
	}
	if (checkReverse(n, k, u, v, icon)) {
		putReverse(u, v, icon);
		return true;
	}
	return false;
}

void checkWinPC() {

	for (int i = 1; i <= turn && winer == 0; i++) {
		int x = D[i].second.first;
		int y = D[i].second.second;
		int icon = D[i].first;
		if (n <= 3 && checkPC(n, 3, x, y, icon)) winer = icon;
		if (n <= 6 && checkPC(n, 4, x, y, icon)) winer = icon;
		if (n >= 7 && checkPC(n, 5, x, y, icon)) winer = icon;
		if (winer > 0) {
			winX = x;
			winY = y;
			//cout << x << " " << y << endl;
			return;
		}
	}
	if (winer == 0 && Save.size() - 1 == n * n) {
		winer = 3;
		return;
	}
	return;
}


#pragma endregion

#pragma region WIN ANIMATION
void Cross(int u, int v, int k) {
	int count1 = 0, count2 = 0;
	for (int i = 0; i < k && u + i <= n && v + i <= n; i++) {
		if (!B1[u + i][v + i]) count1++;
		if (!B2[u + i][v + i]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u + i][v + i].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u + i][v + i].setTexture(&blackOText);
		return;
	}

	count1 = 0; count2 = 0;
	for (int i = 0; i < k && u - i > 0  && v-i> 0; i++) {
		if (!B1[u - i][v - i]) count1++;
		if (!B2[u - i][v - i]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u - i][v - i].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u - i][v - i].setTexture(&blackOText);
		return;
	}
}

void Reverse(int u, int v, int k) {
	int count1 = 0, count2 = 0;
	for (int i = 0; i < k && u - i > 0 && v + i <= n; i++) {
		if (!B1[u - i][v + i]) count1++;
		if (!B2[u - i][v + i]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u - i][v + i].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u - i][v + i].setTexture(&blackOText);
		return;
	}

	count1 = 0; count2 = 0;
	for (int i = 0; i < k && u + i <= n && v - i > 0; i++) {
		if (!B1[u + i][v - i]) count1++;
		if (!B2[u + i][v - i]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u + i][v - i].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u + i][v - i].setTexture(&blackOText);
		return;
	}
}

void Vertical(int u, int v, int k) {

	int count1 = 0, count2 = 0;

	for (int i = 0; i < k && u - i > 0 ; i++) {
		if (!B1[u - i][v]) count1++;
		if (!B2[u - i][v]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u - i][v].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u - i][v].setTexture(&blackOText);
		return;
	}

	count1 = 0; count2 = 0;
	for (int i = 0; i < k && u + i <= n; i++) {
		if (!B1[u + i][v]) count1++;
		if (!B2[u + i][v]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u + i][v].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u + i][v].setTexture(&blackOText);
		return;
	}
}

void Horizontal(int u, int v, int k) {
	int count1 = 0, count2 = 0;
	for (int i = 0; i < k && v + i <= n; i++) {
		if (!B1[u][v + i]) count1++;
		if (!B2[u][v + i]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u][v + i].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u][v + i].setTexture(&blackOText);
		return;
	}

	count1 = 0; count2 = 0;
	for (int i = 0; i < k &&  v - i > 0; i++) {
		if (!B1[u][v - i]) count1++;
		if (!B2[u][v - i]) count2++;
	}
	if (count1 == k)
	{
		for (int i = 0; i < k; i++)
			box[u][v - i].setTexture(&blackXText);
		return;
	}
	if (count2 == k)
	{
		for (int i = 0; i < k; i++)
			box[u][v - i].setTexture(&blackOText);
		return;
	}
}

void putAnimationWin(int u, int v, int k) {
	Cross(u, v, k);
	Reverse(u, v, k);
	Vertical(u, v, k);
	Horizontal(u, v, k);
}

#pragma endregion

bool enterName(sf::Text &name,sf::Event event) {
	sf::String str = name.getString();
	if (str == "| ") str = "";
	if (event.text.unicode == 8) {
		if (str.getSize())
			str.erase(str.getSize() - 1, str.getSize());
		name.setString(str);
		return true;
	}
	if (event.text.unicode != 13)
	{
		str += event.text.unicode;
		name.setString(str);
		return true;
	}
	if (event.text.unicode == 13) {
		return false;
	}
}

int Number(sf::Event event, sf::Text &number) {
	sf::String str = number.getString();
	int num = 0;
	if (str == "|") {
		str = "";
		num = 0;
	}
	else {
		for (int i = 0; i < str.getSize(); i++)
			num = num * 10 + str[i] - '0';
	}
	if (event.text.unicode == 8) {
		if (str.getSize())
			str.erase(str.getSize() - 1, str.getSize());
		number.setString(str);
		//number.setPosition((screenWidth - enterNumber.getGlobalBounds().width) / 2, 450);
		num /= 10;
		return num;
	}
	if (event.text.unicode != 13)
	{
		str += event.text.unicode;
		number.setString(str);
		//number.setPosition((screenWidth - enterNumber.getGlobalBounds().width) / 2, 450);
		num = num * 10 + event.text.unicode - '0';
		return num;
	}
	return num;
}

void saveGamePVP() {
	stringstream str;
	string R = name.getString();
	str <<"Data/PVP/"<<R<<".txt";
	string fileName = str.str();

	ofstream fileOut(fileName.c_str());
	if (fileOut.good() == 1) fileOut.clear();

	fileOut << n << endl;
	fileOut << xScore << " " << oScore << endl;
	string s = player1.getString();
	fileOut << s << endl;
	s = player2.getString();
	fileOut << s << endl;
	fileOut << turn << endl;
	for (int i = 1; i <= turn; i++)
		fileOut << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
	fileOut << 0;
}

void loadGamePVP() {
	
	string R = name.getString();
	stringstream str;
	str << "Data/PVP/" << R << ".txt";
	string fileName = str.str();

	ifstream fileIn(fileName.c_str());

	if (fileIn.good() != 1) {
		loadError = true;
		return;
	}
	if (fileIn.is_open()) {
		fileIn >> n;
		cout << n << endl;
		fileIn >> xScore >> oScore;
		cout << xScore << " " << oScore << endl;

		char s[100];

		fileIn.ignore();

		fileIn.getline(s, 100);
		string r;
		r = s;
		cout << r << endl;
		player1.setString(r);

		fileIn.getline(s, 100);
		r = s;
		cout << r << endl;
		player2.setString(r);

		fileIn >> turn;
		cout << turn;
		for (int i = 1; i <= turn; i++)
		{
			fileIn >> D[i].first >> D[i].second.first >> D[i].second.second;
			int icon = D[i].first;
			int x = D[i].second.first;
			int y = D[i].second.second;
			if (icon == 1) {
				B1[x][y] = false;
				box[x][y].setTexture(&bigXBoxText);
			}
			else {
				B2[x][y] = false;
				box[x][y].setTexture(&bigOBoxText);
			}
			//S.push(make_pair(icon, make_pair(x, y)));
			cout << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
		}
	}
}

void saveGamePVC() {
	stringstream str;
	string R = name.getString();
	str << "Data/PVC/" << R << ".txt";
	string fileName = str.str();

	ofstream fileOut(fileName.c_str());
	if (fileOut.good() == 1) fileOut.clear();

	fileOut << n << endl;
	fileOut << xScore << " " << oScore << endl;
	fileOut << playerIcon << endl;

	string s = player.getString();
	fileOut << s << endl;
	
	fileOut << turn << endl;
	for (int i = 1; i <= turn; i++)
		fileOut << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
	fileOut << 0;
}

void loadGamePVC() {
	string R = name.getString();
	stringstream str;
	str << "Data/PVC/" << R << ".txt";
	string fileName = str.str();

	ifstream fileIn(fileName.c_str());

	if (fileIn.good() != 1) {
		loadError = true;
		return;
	}
	if (fileIn.is_open()) {
		fileIn >> n;
		cout << n << endl;

		fileIn >> xScore >> oScore;
		cout << xScore << " " << oScore << endl;

		fileIn >> playerIcon;
		cout << playerIcon << endl;

		char s[100];

		fileIn.ignore();

		fileIn.getline(s, 100);
		string r;
		r = s;
		cout << r << endl;
		player.setString(r);

		fileIn >> turn;
		cout << turn;
		for (int i = 1; i <= turn; i++)
		{
			fileIn >> D[i].first >> D[i].second.first >> D[i].second.second;
			int icon = D[i].first;
			int x = D[i].second.first;
			int y = D[i].second.second;
			if (icon == 1) {
				B1[x][y] = false;
				box[x][y].setTexture(&bigXBoxText);
			}
			else {
				B2[x][y] = false;
				box[x][y].setTexture(&bigOBoxText);
			}
			//S.push(make_pair(icon, make_pair(x, y)));
			cout << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
		}
	}
}

void saveViewPVP(int number) {
	stringstream str;
	str << "Data/Statistics/PVP/" << number << ".txt";
	string fileName = str.str();
	ofstream fileOut(fileName.c_str());
	fileOut << n << endl;
	fileOut << xScore << " " << oScore << endl;
	string s = player1.getString();
	fileOut << s << endl;
	s = player2.getString();
	fileOut << s << endl;
	fileOut << turn << endl;
	for (int i = 1; i <= turn; i++)
		fileOut << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
	fileOut << winX <<" "<< winY <<" "<< winer<<endl;
}

void saveViewPVC(int number) {
	stringstream str;
	str << "Data/Statistics/PVC/" << number << ".txt";
	string fileName = str.str();
	ofstream fileOut(fileName.c_str());
	fileOut << n << endl;
	fileOut << xScore << " " << oScore << endl;
	fileOut << playerIcon << endl;

	string s = player.getString();
	fileOut << s << endl;

	fileOut << turn << endl;
	for (int i = 1; i <= turn; i++)
		fileOut << D[i].first << " " << D[i].second.first << " " << D[i].second.second << endl;
	fileOut << winX << " " << winY << " " << winer << endl;
}

void updatePVP(int winer) {
	
	ifstream fileIn("Data/Statistics/PVP/summary.txt");
	int countGame, numberOfWinX, numberOfWinO, numberOfDraw;
	fileIn >> countGame;
	fileIn >> numberOfWinX;
	fileIn >> numberOfWinO;
	fileIn >> numberOfDraw;
	cout<< countGame << " " << numberOfWinX << " " << numberOfWinO << " " << numberOfDraw<<endl;
	countGame++;
	if (winer == 1) numberOfWinX++;
	if (winer == 2) numberOfWinO++;
	if (winer == 3) numberOfDraw++;
	cout << countGame << " " << numberOfWinX << " " << numberOfWinO << " " << numberOfDraw<<endl;
	ofstream fileOut("Data/Statistics/PVP/summary.txt");
	fileOut.clear();
	fileOut << countGame << " " << numberOfWinX << " " << numberOfWinO << " " << numberOfDraw;
	saveViewPVP(countGame);

}

void updatePVC(int winer) {
	ifstream fileIn("Data/Statistics/PVC/summary.txt");
	int countGame, numberOfWinX, numberOfWinO, numberOfDraw;
	fileIn >> countGame;
	fileIn >> numberOfWinX;
	fileIn >> numberOfWinO;
	fileIn >> numberOfDraw;
	
	countGame++;
	if (winer == 1) numberOfWinX++;
	if (winer == 2) numberOfWinO++;
	if (winer == 3) numberOfDraw++;
	
	ofstream fileOut("Data/Statistics/PVC/summary.txt");
	fileOut.clear();
	fileOut << countGame<<" "<<numberOfWinX<<" "<<numberOfWinO<<" "<<numberOfDraw;
	saveViewPVC(countGame);
}



int main() {

	//Creation

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Game");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//Input Texture

	loadData();

	//splash screen
	int logoWidth = 117, logoHeight = 95;
	int posX = (screenWidth - logoWidth) / 2, posY = (screenHeight - logoHeight) / 2;
	sf::Sprite logo;
	logo.setTexture(logoText);
	logo.setPosition(posX, posY);

	//Init
	Init();
	//sound
	click.setBuffer(buffer);
	music.setLoop(true);
	//Game loop

	sf::Event event;
	sf::Clock clock;
	clock.restart();
	while (play)
	{
		//SPLASH SCREEN

		logo.setColor(sf::Color(225, 225, 225, alpha));
		if (alpha <= 100) {
			alpha++;
		}
		else if (state == SPLASH) {
			state = MENU;
		}

		//EVENT

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				play = false;
			}
			if (state == MENU && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{

				if (ppMode.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = CHOOSE_SIZE;
					Init();
				}
				if (pc1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = CHOOSE_SIZE;
					Init();
					pcstate = true;
					level = 1;
					
				}
				if (pc2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = CHOOSE_SIZE;
					Init();
					pcstate = true;
					level = 2;
					
				}
				if (pc3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = CHOOSE_SIZE;
					Init();
					pcstate = true;
					level = 3;
					
				}

				if (resume && resumeGame.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = PLAY;
					resume = true;
				}
				if (load.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = LOAD;
					InitForSaveAndLoad();
					enter = false;
					enterNameFile = true;
				}
				if (statistics.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = STATISTIC;
					chooseModeToLoad = false;
					loadPC = false;
				}
				if (aboutUs.getLocalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = ABOUTUS;
				}
			}
			else
			if (state == CHOOSE_SIZE) {

				//Choose Icon
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (numberBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						enter = true;
					}
					if (chooseSizeSucceed && xBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						playerIcon = 1;
						xBox.setTexture(smallXText);
						chooseIconSucceed = true;
					}
					if (chooseSizeSucceed && oBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						playerIcon = 2;
						oBox.setTexture(smallOText);
						chooseIconSucceed = true;
					}

					//Press start
					if (!pcstate && start.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
					{
						state = PLAY;
						xScore = 0;
						oScore = 0;
						score.setString("0 : 0");
						score.setPosition(600 + (200 - score.getGlobalBounds().width) / 2, 468);
						drawableInit = true;
					}
					
					if (pcstate && start.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
					{
						state = PLAY;
						xScore = 0;
						oScore = 0;
						score.setString("0 : 0");
						score.setPosition(600 + (200 - score.getGlobalBounds().width) / 2, 468);
						drawableInit = true;
					}
				}
					
				// Choose size of gameboard
				if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode == 8) {
						n /= 10;
						if (playInput.getSize())
							playInput.erase(playInput.getSize() - 1, playInput.getSize());
						playText.setString(playInput);
					}
					else
						if (event.text.unicode != 13)
						{
							n = n * 10 + event.text.unicode - '0';
							playInput += event.text.unicode;
							playText.setString(playInput);
						}
						else {
							if (n > 20 || n < 3)
							{
								playInput = "";
								playText.setString(playInput);
								n = 0;
							}
							else {
								chooseSizeSucceed = true;
								noti.setString("The size of gameboard: " + playInput + "x" + playInput + ".");
							}
						}
				}		
			}
			else
			if (state == PLAY  )
			{
				//u, v;
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					for (i = 1; i <= n && winer == 0; i++)
						for (j = 1; j <= n && winer == 0; j++)
						{
							if (B[1][i][j] && B[2][i][j] && B1[i][j] && B2[i][j] && box[i][j].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
								u = i;
								v = j;
								click.play();
								if (!pcstate) {
									if (player1Turn) B1[u][v] = false;
									else B2[u][v] = false;
									S.push(make_pair(player1Turn ? 1 : 2, make_pair(u, v)));
									Save.push(make_pair(player1Turn ? 1 : 2, make_pair(u, v)));
									turn++;
									D[turn] = make_pair(player1Turn ? 1 : 2, make_pair(u, v));
									player1Turn = !player1Turn;
									//cout << turn << endl;
								}
								if (pcstate) {
									B[playerIcon][u][v] = false;
									S.push(make_pair(playerIcon, make_pair(u, v)));
									Save.push(make_pair(playerIcon, make_pair(u, v)));
									turn++;
									D[turn] = make_pair(playerIcon, make_pair(u, v));
									if (winer == 0) checkWinPC();
									if (winer > 0) {
										clock.restart();
										if (winer == 1) xScore++; else oScore++;
										updatePVC(winer);
									}
									if (winer == 0) {
										if (n <= 3) playLevel(playerIcon, n, 3, level);
										else if (n <= 6) playLevel(playerIcon, n, 4, level);
										else playLevel(playerIcon, n, 5, level);
										clock.restart();
									}
								}
								break;
							}
						}

					if (undo.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						undostate = true;
					}
					if (!pcstate && !undostate && winer == 0)
					{
						for (int i = turn; i >= 1 && winer == 0; i--) {
							int x = D[i].second.first;
							int y = D[i].second.second;
							//cout << x << " " << y << endl;
							if (n <= 3) winer = check(n, 3, x, y);
							else if (n <= 6) winer = check(n, 4, x, y);
							else winer = check(n, 5, x, y);
							winX = x;
							winY = y;
							if (winer > 0) {
								clock.restart();
								if (winer == 1) xScore++; else oScore++;
								updatePVP(winer);
							}
						}
					}
					
					if (pcstate && !undostate  && winer == 0) {
						checkWinPC();
						if (winer > 0) {
							clock.restart();
							if (winer == 2) oScore++; else xScore++;
							updatePVC(winer);
						}
					}
					
					if (sound.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						if (playMusic) {
							playMusic = false;
							sound.setTexture(soundOffText);
							music.pause();
						}
						else {
							playMusic = true;
							sound.setTexture(soundOnText);
							music.play();
						}
					}

					if (back.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						state = MENU;
						resume = true;
						music.pause();
						playMusic = false;
						sound.setTexture(soundOffText);
					}
					
					if (clock.getElapsedTime().asSeconds() >= 1.0f && pcstate && winer > 0 && continuePlay.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						still = true;
						Init();
						pcstate = true;

					}

					if (clock.getElapsedTime().asSeconds() >= 1.0f && !pcstate && winer > 0 && continuePlay.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						still = true;
						Init();
						pcstate = false;
						playerIcon = 0;
					}
					else
						if (clock.getElapsedTime().asSeconds() >= 1.0f && winer > 0 && exitBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
							still = false;
							winer = 0; n = 0;
							state = MENU;
							resume = false;
							playerIcon = 0;
							pcstate = false;
						}
						else
							if (winer == 0)
							{
								if (turn == n * n) {
									winer = 3;
									if (pcstate) updatePVC(winer); else updatePVP(winer);
								}
							}

					if (!pcstate && player1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						player1.setString("| ");
						enterTextPlayer1 = true;
					}
					if (!pcstate && player2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						player2.setString("| ");
						enterTextPlayer2 = true;
					}
					if (pcstate && player.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						player.setString("| ");
						enterTextPlayer = true;
					}
					if (save.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						state = SAVE;
						InitForSaveAndLoad();
						loadPC = pcstate;
						enter = false;
						enterNameFile = true;
					}

				}
				if (event.type == sf::Event::TextEntered) {
					if (enterTextPlayer1) enterTextPlayer1 = enterName(player1,event);
					if (enterTextPlayer2) enterTextPlayer2 = enterName(player2,event);
					if (enterTextPlayer)  enterTextPlayer = enterName(player, event);
				}
			}
			else 
			if (state == SAVE) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (boxName1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						enter = true;
						enterNameFile = true;
					}
					if (saveButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						state = PLAY;
						if (!pcstate) saveGamePVP();
						else {
							saveGamePVC();
							pcstate = true;
						}
					}
				}
				if (enterNameFile && event.type == sf::Event::TextEntered) {
					enterNameFile = enterName(name, event);
				}
				if (!enterNameFile) {
					enterNameSucceed = true;
				}
			}
			else
			if (state == LOAD) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (!chooseModeToLoad) {
						if (ppMode.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
							loadPC = false;
							chooseModeToLoad = true;
						}
						if (pcMode.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
							loadPC = true;
							chooseModeToLoad = true;
						}
					}
					else {
						if (boxName1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
							enter = true;
							enterNameFile = true;
						}
						if (loadButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
							state = PLAY;
							if (!loadPC) {
								loadGamePVP();
								if (!loadError) {
									Init();
									loadGamePVP();
									state = PLAY;
									drawableInit = true;
								}
								else {
									state = LOAD;
									InitForSaveAndLoad();
								}
							}
							else {
								loadGamePVC();
								if (!loadError) {
									Init();
									pcstate = true;
									loadGamePVC();
									state = PLAY;
									drawableInit = true;
								}
								else {
									state = LOAD;
									InitForSaveAndLoad();
								}
							}
						}
					}
				}
				if (enterNameFile && event.type == sf::Event::TextEntered) {
					enterNameFile = enterName(name, event);
				}
				if (!enterNameFile) {
					enterNameSucceed = true;
				}
			}
			else
			if (state == STATISTIC) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
					if (!chooseModeToLoad) {
						if (ppMode.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
							loadPC = false;
							chooseModeToLoad = true;
							InitForStatistic(0);
						}
						if (pcMode.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
							loadPC = true;
							chooseModeToLoad = true;
							InitForStatistic(1);
						}
					}
					if (back.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						state = MENU;
					}
				}
				if (chooseModeToLoad && event.type == sf::Event::TextEntered) {
					int num = Number(event, enterNumber);
					cout << num<<endl;
					if (event.text.unicode == 13) {
						if (num != 0 & num <= GAMES) {
							if (loadPC) {
								Init();
								InitForViewPVC(num);
								state = VIEW;
								drawableInit = true;
							}
							else {
								Init();
								InitForViewPVP(num);
								state = VIEW;
								drawableInit = true;
							}
						}
						else {
							if (loadPC) InitForStatistic(1); else  InitForStatistic(0);
							chooseNumberSucceed = false;
						}
					}
				}
			}
			else 
			if (state == VIEW) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (sound.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						if (playMusic) {
							playMusic = false;
							sound.setTexture(soundOffText);
							music.pause();
						}
						else {
							playMusic = true;
							sound.setTexture(soundOnText);
							music.play();
						}
					}

					if (back.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						state = MENU;
						resume = true;
						music.pause();
						playMusic = false;
						sound.setTexture(soundOffText);
					}
				}
			}
			else 
			if (state == ABOUTUS && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if (back.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					state = MENU;
				}
			}
		}
		
#pragma region LOGIC
		//LOGIC
		
		if (state == MENU)
		{
			if (!resume) Init();
		}
		else
		if (state == CHOOSE_SIZE) {
				xBox.setPosition(294, 408);
				oBox.setPosition(456, 408);
		}
		else
		if (state == PLAY) {
			stringstream text ;
			text << xScore << " : " << oScore;
			score.setString(text.str());
			score.setPosition(600 + (200 - score.getGlobalBounds().width) / 2, 468);
			winNoti.setPosition((600 - winNoti.getGlobalBounds().width) / 2, 232);
			boxSize = screenHeight / n;
			board = n * boxSize;
			gameBoard.setSize(sf::Vector2f(board, board));
			gameBoard.setPosition(0, (screenHeight - board) / 2);
			information.setSize(sf::Vector2f((screenWidth - board), screenHeight));
			information.setPosition(board, 0);
			X = 0, Y = (screenHeight - board) / 2;
			if (drawableInit) {
				for (i = 1; i <= n; i++)
				{
					for (j = 1; j <= n; j++)
					{
						box[i][j].setSize(sf::Vector2f(boxSize, boxSize));
						box[i][j].setPosition(X, Y);
						X = X + boxSize;
					}
					Y = Y + boxSize;
					X = 0;
				}
				drawableInit = false;
			}
			oBox.setPosition(615, 122);
			xBox.setPosition(615, 42);
			oBox.setTexture(oBoxText);
			xBox.setTexture(xBoxText);

			//PvC
			if (pcstate) {
				if (playerIcon == 1) {
					player.setPosition(684, 50);
					computer.setPosition(684, 140);
				}
				else {
					player.setPosition(684, 140);
					computer.setPosition(684, 50);
				}

				doIt(undostate, playerIcon);

				//Time delay computer play 
				if (turn % 2 == 0) {
					int x = D[turn].second.first;
					int y = D[turn].second.second;
					if (clock.getElapsedTime().asSeconds() >= 0.5f) {
						if (D[turn].first == 1) {
							if (winer > 0 && winer != 3) box[x][y].setTexture(&blackXText);
							else box[x][y].setTexture(&bigXBoxText);
						}
						else {
							if (winer > 0 && winer != 3) box[x][y].setTexture(&blackOText);
							else box[x][y].setTexture(&bigOBoxText);
						}
					}
					else box[x][y].setTexture(&emptyText);
				}
				if (turn % 2 == 1 && winer > 0) {
					if (D[turn].first == 1) {
						box[D[turn].second.first][D[turn].second.second].setTexture(&blackXText);
					}
					else box[D[turn].second.first][D[turn].second.second].setTexture(&blackOText);
				}
			}

			//PvP
			if (!pcstate) {

				//Undo
				if (undostate && !Save.empty()) {
					int x = Save.top().second.first;
					int y = Save.top().second.second;
					box[x][y].setTexture(&emptyText);
					undostate = false;
					turn--;
					if (Save.top().first == 1) {
						B1[x][y] = true;
						B2[x][y] = true;
						player1Turn = true;
					}
					else {
						B2[x][y] = true;
						B1[x][y] = true;
						player1Turn = false;
					}
					Save.pop();
				}

				if (player1Turn) {
					player1.setStyle(sf::Text::Underlined);
					player2.setStyle(sf::Text::Regular);
				}
				else {
					player1.setStyle(sf::Text::Regular);
					player2.setStyle(sf::Text::Underlined);
				}

				while (!S.empty()) {
					int u = S.top().second.first;
					int v = S.top().second.second;
					if (S.top().first == 1) {
						box[u][v].setTexture(&bigXBoxText);
					}
					else
					{
						box[u][v].setTexture(&bigOBoxText);
					}
					S.pop();
				}


				if (winer > 0) {
					//Animation win 

					if (winer != 3) {
						if (n <= 3) putAnimationWin(winX, winY, 3);
						else if (n <= 6) putAnimationWin(winX, winY, 4);
						else putAnimationWin(winX, winY, 5);

						//cout << winX << " " << winY << endl;
					}
					
					if (winer == 1) {
						sf::String str;
						str = player1.getString() + " Win";
						winNoti.setString(str);
					}
					else
						if (winer == 2) {
							sf::String str;
							str = player2.getString() + " Win";
							winNoti.setString(str);
						}
						else
						{
							winNoti.setString("DRAW!");
							winNoti.setPosition(250, 232);
						}
				}
			}

		}
		else 
		if (state == SAVE) {
			saveButton.setPosition((screenWidth - saveButton.getGlobalBounds().width) / 2, 550);
			asking.setString("Enter a name to save: ");
			asking.setPosition((screenWidth - asking.getGlobalBounds().width) / 2, 114);
			name.setPosition((screenWidth - name.getGlobalBounds().width) / 2, 220);
			if (enterNameSucceed) {
				nameNoti.setString("Your file game is :" + name.getString() + ".txt");
				nameNoti.setPosition((screenWidth - nameNoti.getGlobalBounds().width) / 2, 275);
			}
		}
		else
		if (state == LOAD) {
			if (loadError) {
				nameNoti.setString("Not found. Enter again!");
				nameNoti.setPosition((screenWidth - nameNoti.getGlobalBounds().width) / 2, 275);
			}
			asking.setString("Enter a name to load: ");
			asking.setPosition((screenWidth - asking.getGlobalBounds().width) / 2, 114);
			name.setPosition((screenWidth - name.getGlobalBounds().width) / 2, 220);
			if (enterNameSucceed && !loadError) {
				nameNoti.setString("The game you want to play: " + name.getString());
				nameNoti.setPosition((screenWidth - nameNoti.getGlobalBounds().width) / 2, 275);
			}
		}
		else 
		if (state == STATISTIC) {
			if (chooseModeToLoad) {
				numberOfGame.setPosition((screenWidth - numberOfGame.getGlobalBounds().width) / 2, 80);
				xWin.setPosition((screenWidth - xWin.getGlobalBounds().width) / 2, 150);
				oWin.setPosition((screenWidth - oWin.getGlobalBounds().width) / 2, 230);
				draw.setPosition((screenWidth - draw.getGlobalBounds().width) / 2, 310);
				enterNumber.setPosition((screenWidth - enterNumber.getGlobalBounds().width) / 2, 450);
				back.setPosition((screenWidth - back.getGlobalBounds().width), screenHeight - back.getGlobalBounds().height);
				numberBox2.setPosition((screenWidth - numberBox2.getLocalBounds().width) / 2, 450);
			}
		}
		else 
		if (state == VIEW) {
			stringstream text;
			text << xScore << " : " << oScore;
			score.setString(text.str());
			score.setPosition(600 + (200 - score.getGlobalBounds().width) / 2, 468);
			winNoti.setPosition((600 - winNoti.getGlobalBounds().width) / 2, 232);
			boxSize = screenHeight / n;
			board = n * boxSize;
			gameBoard.setSize(sf::Vector2f(board, board));
			gameBoard.setPosition(0, (screenHeight - board) / 2);
			information.setSize(sf::Vector2f((screenWidth - board), screenHeight));
			information.setPosition(board, 0);
			X = 0, Y = (screenHeight - board) / 2;
			if (drawableInit) {
				for (i = 1; i <= n; i++)
				{
					for (j = 1; j <= n; j++)
					{
						box[i][j].setSize(sf::Vector2f(boxSize, boxSize));
						box[i][j].setPosition(X, Y);
						X = X + boxSize;
					}
					Y = Y + boxSize;
					X = 0;
				}
				drawableInit = false;
			}
			oBox.setPosition(615, 122);
			xBox.setPosition(615, 42);
			oBox.setTexture(oBoxText);
			xBox.setTexture(xBoxText);

			//PvC
			if (pcstate) {
				if (playerIcon == 1) {
					player.setPosition(684, 50);
					computer.setPosition(684, 140);
				}
				else {
					player.setPosition(684, 140);
					computer.setPosition(684, 50);
				}
			}


		}
		else 
		if (state == ABOUTUS) {
			back.setPosition(screenWidth - back.getGlobalBounds().width, screenHeight - back.getGlobalBounds().height);
		}
#pragma endregion 

		//RENDERING

		window.clear();

		switch (state)
		{
		case SPLASH:
			window.draw(logo);
			break;
		case MENU:
			window.draw(gameName);
			window.draw(ppMode);
			window.draw(pcMode);
			window.draw(pc1);
			window.draw(pc2);
			window.draw(pc3);
			window.draw(resumeGame);
			window.draw(statistics);
			window.draw(aboutUs);
			window.draw(load);
			break;
		case CHOOSE_SIZE:
			window.draw(chooseSize);
			if (!enter) window.draw(numberBox);
			else {
				window.draw(numberBox2);
				window.draw(playText);
			}
			if (chooseSizeSucceed) {
				window.draw(noti);

			}
			if (pcstate)
			{
				window.draw(chooseIcon);
				window.draw(xBox);
				window.draw(oBox);
			}
			if (!pcstate && chooseSizeSucceed) {
				window.draw(start);
			}
			if (pcstate && chooseIconSucceed && chooseSizeSucceed) {
				window.draw(start);
			}
			break;
		case PLAY:
			window.draw(information);
			window.draw(gameBoard);
			for (i = 1; i <= n; i++)
				for (j = 1; j <= n; j++)
				{
					window.draw(box[i][j]);
				}
			window.draw(sound);
			window.draw(undo);
			window.draw(back);
			window.draw(save);
			window.draw(xBox);
			window.draw(oBox);
			window.draw(scoreBoard);
			window.draw(score);
			if (!pcstate) {
				window.draw(player1);
				window.draw(player2);
			}
			else {
				window.draw(player);
				window.draw(computer);
			}
			if (n <= 3) window.draw(rule1);
			else if (n <= 6) window.draw(rule2);
			else window.draw(rule3);
			if (winer > 0) {
				if (clock.getElapsedTime().asSeconds() >= 1.0f) {
					window.draw(win);
					window.draw(continuePlay);
					window.draw(exitBox);
					window.draw(winNoti);
				}
			}
			break;
		
		case CHOOSEMODE:
			window.draw(ppMode);
			window.draw(pcMode);
			break;
		case SAVE:
			window.draw(asking);
			if (!enter) window.draw(boxName1);
			else {
				window.draw(boxName2);
				window.draw(name);
			}
			if (enterNameSucceed) {
				window.draw(nameNoti);
				window.draw(saveButton);
			}
			break;
		case LOAD:
			if (!chooseModeToLoad) {
				window.draw(pcMode);
				window.draw(ppMode);
			}
			else {
				window.draw(asking);
				if (!enter) window.draw(boxName1);
				else {
					window.draw(boxName2);
					window.draw(name);
				}
				if (enterNameSucceed) {
					window.draw(nameNoti);
					window.draw(loadButton);
				}
			}
			break;
		case STATISTIC:
			if (!chooseModeToLoad) {
				window.draw(pcMode);
				window.draw(ppMode);
			}
			else {
				window.draw(numberOfGame);
				window.draw(xWin);
				window.draw(oWin);
				window.draw(draw);
				window.draw(numberBox2);
				//window.draw(loadButton);
				window.draw(back);
				window.draw(enterNumber);
				//window.draw(number);	
			}
			break;
		case VIEW:
			window.draw(gameBoard);
			for (i = 1; i <= n; i++)
				for (j = 1; j <= n; j++)
				{
					window.draw(box[i][j]);
				}
			window.draw(sound);
			window.draw(undo);
			window.draw(back);
			window.draw(save);
			window.draw(xBox);
			window.draw(oBox);
			window.draw(scoreBoard);
			window.draw(score);
			if (!pcstate) {
				window.draw(player1);
				window.draw(player2);
			}
			else {
				window.draw(player);
				window.draw(computer);
			}
			if (n <= 3) window.draw(rule1);
			else if (n <= 6) window.draw(rule2);
			else window.draw(rule3);
			break;
		case ABOUTUS:
			window.draw(back);
			break;
		}
			
		window.display();
	}

	//CLean up

	window.close();

	return 0;
}