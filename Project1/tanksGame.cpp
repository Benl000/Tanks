#include "tanksGame.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
using std::cout;
using std::endl;
using std::string;

void tanksGame::run(GameMode runMode) // Modified signature
{
	currentRunMode = runMode; // Store the mode
	setMode(ON); // Initial status for menu control



	if (currentRunMode == GameMode::LOAD || currentRunMode == GameMode::SILENT_LOAD) {
		// In LOAD or SILENT_LOAD mode, there is NO menu.
		// Directly initialize and run the game from files.
		setMap(FILE); // Indicate that map source is from file
		init(); // This will call game.initFromFile()
	}
	else {
		// In NORMAL or SAVE mode, show the main menu.
		mainMenu();
	}
}

void tanksGame::init()
{
	setMode(PLAY);

	// Determine the screen name. You'll need a mechanism for this.
	// For now, let's hardcode for simplicity or assume it's derived from map selection.
	// If your printMapSelection uses specific screen files, you'd get the name there.
	// For example, if '1' loads "tanks-game_01.screen", then currentScreenBaseName = "tanks-game_01";
	// Let's assume you have a way to get it, or you'll need to add a mechanism.
	// For LOAD/SAVE mode, we'll assume a default screen name for testing.
	if (currentRunMode == GameMode::LOAD || currentRunMode == GameMode::SILENT_LOAD || currentRunMode == GameMode::SAVE) {
		// You'll need to determine this dynamically based on which screen is being played.
		// For testing, let's assume `tanks-game_01` is the screen being loaded/saved.
		// In a real scenario, this would come from a map selection or a command-line arg.
		currentScreenBaseName = "tanks-game_01"; // Placeholder
		// Example: If you select a map "tanks-game_02.screen", then set currentScreenBaseName = "tanks-game_02";
	}


	if (currentRunMode == GameMode::SAVE) {
		unsigned int seed = static_cast<unsigned int>(time(NULL));
		srand(seed); // Initialize RNG with a new seed for recording

		// 1. ENABLE RECORDING FIRST
		recorder.setRecordingEnabled(true);

		// 2. THEN, ATTEMPT TO START RECORDING (OPEN FILES)
		// Crucially, check its return value. If it fails, disable recording.
		if (!recorder.startRecording(currentScreenBaseName, seed, 0)) {
			std::cerr << "ERROR: Failed to initialize GameRecorder for SAVE mode. Recording will be disabled." << std::endl;
			recorder.setRecordingEnabled(false); // Disable recording if file opening failed
			// Optional: If saving is critical, you might want to switch modes or exit here.
			// currentRunMode = GameMode::NORMAL; // Fallback to normal if saving fails
		}
		else {
			std::cout << "GameRecorder enabled and files opened for SAVE mode: " << currentScreenBaseName << std::endl;
		}
	}
	if (currentRunMode == GameMode::NORMAL) {
		unsigned int seed = static_cast<unsigned int>(time(NULL));
		srand(seed); // Initialize RNG with a new seed for recording
	}
	
	else if (currentRunMode == GameMode::LOAD || currentRunMode == GameMode::SILENT_LOAD) {
		// Load steps and expected results
		loader.loadScreenData(currentScreenBaseName);
		// Initialize RNG with the loaded seed from the .steps file
		srand(loader.getLoadedSeed());
		game.initFromFile(); // Game should initialize based on the map file, but its random elements use the loaded seed.
		// NOTE: This `initFromFile` might need to be adapted to rely on `loader` for initial tank/cannon positions
		// if they are randomized and need to be reproducible from the seed.
	}
	if (currentRunMode == GameMode::SAVE || currentRunMode == GameMode::NORMAL) { 
		switch (source) {
		case RANDOM:
			game.initRandom(); // Standard random init
			break;
		case FILE:
			game.initFromFile(); // Standard file init
			break;
		}
	}

	// Render all is only needed if not in silent mode
	if (currentRunMode != GameMode::SILENT_LOAD) {
		game.renderAll();
	}

	gameLoop();
}

void tanksGame::gameLoop()
{
	int currentGameTime = 0; // Introduce a game time counter

	while (mode == PLAY) {

		// --- Input Handling / Action Application ---
		if (currentRunMode == GameMode::LOAD || currentRunMode == GameMode::SILENT_LOAD) {
			// In load/silent mode, apply steps from the loaded file - not working yet
			//loader.applyStepsForCurrentTime(currentGameTime, game);
		}
		else {
			// In normal/save mode, handle user input
			// User input handler will also record if in SAVE mode
			for (int i = 0; i < game.getPlayersAmount(); ++i) {
				Player& player = game.getPlayer(i);
				if (player.isComputer()) {
					game.handleComputerTurn(player, i, currentGameTime, recorder);
				}
				else {
					handleInput();
				}
			}

			//  Global ESC check — works in CvC mode too
			if (_kbhit()) {
				char key = tolower(_getch());
				if (key == 27) { // ESC
					mode = ON;
					mainMenu();
					return;
				}
			}

		}

		// --- Pause Handling (only in non-silent modes) ---
		if (currentRunMode != GameMode::SILENT_LOAD) {
			if (mode == PAUSE) {
				handlePause(); // Manage pause screen
			}
		}


		// --- Game Logic Updates ---
		if (mode == PLAY) { // Only update if not paused
			game.updateShells(recorder, currentGameTime); // Move shells, handle collisions

			// Speed adjustment for non-silent load
			if (currentRunMode == GameMode::LOAD) {
				Sleep(speed / 4); // Faster than normal, but still visible
			}
			else if (currentRunMode == GameMode::NORMAL || currentRunMode == GameMode::SAVE) {
				Sleep(speed / 2); // Normal speed
			}

			game.moveTanks(recorder, currentGameTime); // Move tanks

			// If in SAVE mode, record tank movements (direction changes)
			// This would typically be inside `handleInput` or `game.moveTanks` or `game.getPlayer().handleInput`
			// where player/AI input changes tank direction.
			// Example: `recorder.recordMove(currentGameTime, playerID, tankID, newDirection);`

			game.updateShells(recorder, currentGameTime); // Update shells again after tank movement if needed

			// If in SAVE mode, record hits and deaths from this second updateShells if needed
			// Example: `game.checkCollisions(recorder);`
		}

		// --- Sleep (controlled by game mode) ---
		if (currentRunMode == GameMode::LOAD) {
			Sleep(speed / 4); // Faster playback
		}
		else if (currentRunMode == GameMode::NORMAL || currentRunMode == GameMode::SAVE) {
			Sleep(speed / 2); // Normal speed
		}
		else if (currentRunMode == GameMode::SILENT_LOAD) {
			// No sleep or minimal sleep for silent mode
			// Sleep(0); // Or a very small non-zero value if Sleep(0) causes issues
		}

		// --- Game Over Check ---
		if (game.checkGameOver()) {
			// If in SAVE mode, record final scores
			if (currentRunMode == GameMode::SAVE) {
				recorder.recordScores(game.players[0].getScore(), game.players[1].getScore()); // Assuming scores are available
				recorder.stopRecording(); // Close files
			}
			// If in SILENT_LOAD mode, compare results
			if (currentRunMode == GameMode::SILENT_LOAD) {
				// This is where you'd perform the comparison logic from Step 6.
				// You need to pass actual results (captured during the silent run)
				// and the expected results (from loader) to a comparison function.
				// This part needs the 'actualResults' captured by `Game` and passed back.
				// For example: `bool testResult = compareResults(game.getActualResults(), loader.getExpectedResults());`
				// Then print "Test Passed" or "Test Failed"
				std::cout << "Silent Test for screen " << currentScreenBaseName << " - ";
				// Dummy comparison for now - replace with actual logic
				if (loader.getExpectedResults().empty()) { // Placeholder logic
					std::cout << "Test PASSED (No expected results loaded or no actual results captured)." << std::endl;
				}
				else {
					// Implement actual comparison logic here
					std::cout << "Test FAILED (Comparison logic not yet implemented)." << std::endl;
				}
			}
			break; // Exit gameLoop
		}
		currentGameTime++; // Increment game time
	}
	mode = ON; // Back to menu mode
	// Only wait for _getch() if not in silent mode
	if (currentRunMode != GameMode::SILENT_LOAD) {
		_getch(); // wait for the player to continue after game over
	}
}

void tanksGame::handleInput()
{
	while (_kbhit()) {
		char key = tolower(_getch());

		for (int i = 0; i < game.getPlayersAmount(); ++i) {
			game.getPlayer(i).handleInput(key, game.getShells(), i);

		}
		if (key == 27) mode = PAUSE;
	}
}

void tanksGame::printPauseScreen() {
	system("cls"); // Clear screen like other menus

	int centerX = 40; // Assuming 80-width console
	int y = 8;

	// Title
	setColorByName("bright yellow");
	gotoxy(centerX - 10, y);
	cout << "===== Game Paused =====";

	// Instruction: Resume
	setColorByName("bright cyan");
	gotoxy(centerX - 10, y + 2);
	cout << "(ESC) Resume Game";

	// Instruction: Main Menu
	setColorByName("bright magenta");
	gotoxy(centerX - 10, y + 3);
	cout << "(X) Return to Main Menu";

	// Instruction: Quit Option (optional)
	setColorByName("bright red");
	gotoxy(centerX - 10, y + 5);
	cout << "Game is paused. Make your choice.";

	resetColor();
}


void tanksGame::handlePause()
{
	printPauseScreen();

	// Handle pause interaction
	while (mode == PAUSE) {
		if (_kbhit()) {
			char key = tolower(_getch());
			switch (key) {
			case 27: // ESC key
				mode = PLAY;
				game.renderAll();
				return;
			case 'x':
				mode = ON;
				mainMenu();
				return;
			}
		}
		Sleep(50);
	}
}


void tanksGame::setMode(Status s)
{
	mode = s;
}

void tanksGame::mainMenu()
{
	while (mode == ON) {
		printMainMenu();
		char choice = tolower(_getch());
		switch (choice) {
		case '1':
			printMapSelection();
			break;
		case '7':
			printSettings();
			break;  // No need for printMainMenu() here
		case '8':
			printInstructions();
			break;  // No need for printMainMenu() here
		case '9':
			setMode(OFF);
			break;
		default:
			break;
		}
	}
}

void tanksGame::printMainMenu() {

	system("cls");

	// Render the main menu with a stylish ice-cream theme
	setColorByName("bright yellow");
	gotoxy(30, 8);
	cout << "===== Tank Battle =====";

	setColorByName("bright cyan");
	gotoxy(30, 10);
	cout << "(1) Start a New Game";

	setColorByName("bright magenta");
	gotoxy(30, 11);
	cout << "(7) Settings";

	setColorByName("bright green");
	gotoxy(30, 12);
	cout << "(8) Instructions";

	setColorByName("bright red");
	gotoxy(30, 14);
	cout << "(9) Exit";

	resetColor();
}

void tanksGame::printInstructions()
{
	game.initPlayersData();

	system("cls");

	// Calculate the centered X position
	int centerX = (Game::WIDTH / 2);
	int startY = 4;

	// Render the instructions title with a colorful border
	setColorByName("bright yellow");
	gotoxy(centerX - 18, startY - 2);
	cout << "   ======== Instructions =========";

	for (int i = 0; i < game.getPlayersAmount(); i++) {
		Player& p = game.getPlayer(i);
		Player::ControlKeys& keys = p.getControls();

		// Player title with player color
		setColorByName("bright cyan");
		gotoxy(centerX - 15, startY);
		cout << "Player " << i + 1 << " Controls:";

		// Player controls in aligned format
		setColorByName("bright magenta");
		gotoxy(centerX - 15, startY + 1);
		cout << " " << keys.leftForward << " - Left track forward/stop";
		gotoxy(centerX - 15, startY + 2);
		cout << " " << keys.leftBackward << " - Left track backward/stop";
		gotoxy(centerX - 15, startY + 3);
		cout << " " << keys.rightForward << " - Right track forward/stop";
		gotoxy(centerX - 15, startY + 4);
		cout << " " << keys.rightBackward << " - Right track backward/stop";
		gotoxy(centerX - 15, startY + 5);
		cout << " " << keys.stopBoth << " - Stop both tracks";
		gotoxy(centerX - 15, startY + 6);
		cout << " " << keys.shoot << " - Shoot";
		gotoxy(centerX - 15, startY + 7);
		cout << " " << keys.switchActiveTank << " - Switch Active Tank";

		// Adjust starting position for the next player
		startY += 9; // 8 lines for each player + 1 space
	}

	// Render the exit instruction with a stylish frame
	setColorByName("bright red");
	gotoxy(centerX - 15, startY );
	cout << "(ESC) Exit";
	resetColor();

	// Input handling
	while (true) {
		if (_kbhit()) {
			char key = tolower(_getch());
			if (key == 27) { // ESC
				system("cls");
				break;
			}
		}
	}
}

void tanksGame::printSettings()
{
	while (true) {
		system("cls");
		int centerX = 20;
		// Render the settings title
		setColorByName("bright yellow");
		gotoxy(centerX, 8);
		cout << "=============== Settings ===============";

		setColorByName("bright cyan");
		gotoxy(centerX, 10);
		cout << "(1) Number of tanks per player:        " << game.getTanksPerPlayer();

		setColorByName("bright magenta");
		gotoxy(centerX, 11);
		cout << "(2) Colored game:                    " << (game.getColorMode() ? " ON" : "OFF");

		setColorByName("bright green");
		gotoxy(centerX, 12);
		cout << "(3) Players mode: ";
		switch (game.getPlayerStatus()) {
		case 2:
			cout << "    Human vs. Computer ";
			break;
		case 3:
			cout << " Computer vs. Computer";
			break;
		case 1:
		default:
			cout << "       Human vs. Human";
			break;
		}

		setColorByName("bright blue");
		gotoxy(centerX, 13);
		cout << "(4) Cluster density: ";
		switch (game.getClusterSize()) {
		case 0:
			cout << "               ---- ";
			break;
		case 1:
			cout << "               #--- ";
			break;
		case 2:
			cout << "               ##-- ";
			break;
		case 3:
			cout << "               ###- ";
			break;
		case 4:
		default:
			cout << "               #### ";
			break;
		}

		setColorByName("bright red");
		gotoxy(centerX+15,15);
		cout << "(ESC) Exit";

		resetColor();

		char key = tolower(_getch());
		switch (key) {
		case 27: // ESC
			return;
		case '1':
			game.setTanksPerPlayer();
			break;
		case '2':
			game.setColorMode();
			break;
		case '3':
			game.setPlayersMode();
			break;
		case '4':
			game.setClusterSize();
			break;
		default:
			break;
		}
	}
}

void tanksGame::printMapSelection() {
	system("cls");
	// Calculate the centered X position
	int centerX = 40; // Assuming a console width of 80

	// Render the map selection title
	setColorByName("bright yellow");
	gotoxy(centerX - 10, 8);
	cout << "===== Choose Map =====";

	// Map options with clear alignment
	setColorByName("bright cyan");
	gotoxy(centerX - 10, 10);
	cout << "(1) Map from file";

	setColorByName("bright magenta");
	gotoxy(centerX - 10, 11);
	cout << "(2) Random";

	setColorByName("bright red");
	gotoxy(centerX - 10, 13);
	cout << "(ESC) Exit";

	resetColor();

	// Handle input for map selection
	char key = tolower(_getch());
	switch (key) {
	case 27: // ESC
		return;
	case '1':
		setMap(FILE);
		init();
		break;
	case '2':
		setMap(RANDOM);
		init();
		break;
	default:
		printMapSelection(); // Re-render the menu on invalid input
		break;
	}
}

void tanksGame::setMap(Map choice)
{
	source = choice;
}
