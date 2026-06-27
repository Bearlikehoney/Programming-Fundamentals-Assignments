/*
   Course: CCP6114 Programming Fundamentals
   Lecture Class: TC1L
   Tutorial Class: TT4L
   Trimester: 2520
   Information :
   0000000000 | ZUL FADHLI ZAIMAN | ZUL.FADHLI.ZAIMAN1@STUDENT.MMU.EDU.MY
   253UC255Q0 | GHITH EMAM | EMAM.GHITH@STUDENT.MMU.EDY.MY
   261UC260WG | RINAD ABOBAKER MOHAMED | RINAD.ABOBAKER.MOHAMED@STUDENT.MMU.EDU.MY
   261UC2610E | FAISA HUSSEIN ABDULLAHI | HUSSEIN.FAISA.ABDULLAHI@STUDENT.MMU.STUDENT.MY
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <limits>   // for numeric_limits

using namespace std;

// دالة لإنشاء الرقعة وحجز الذاكرة ديناميكياً باستخدام المؤشرات (Pointers)
char** createBoard(int size) {
    char** board = new char*[size];
    for (int i = 0; i < size; i++) {
        board[i] = new char[size];
    }
    return board;
}

// دالة لتوزيع القطع الابتدائي بالتناوب (بلوك فيه قطعة وبلوك لا - رقعة الشطرنج)
void initializeBoard(char** board, int size) {
    int rowsOfPieces = (size == 6) ? 2 : 3;

    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if ((r + c) % 2 == 1) {
                if (r < rowsOfPieces) {
                    board[r][c] = 'O';
                } else if (r >= size - rowsOfPieces) {
                    board[r][c] = 'X';
                } else {
                    board[r][c] = ' ';
                }
            } else {
                board[r][c] = ' ';
            }
        }
    }
}

// دالة طباعة البوردة بالخطوط والمسافات الرأسية والأفقية لتطابق الـ Sample تماماً
void displayBoard(char** board, int size) {
    cout << "\n";
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            cout << "|-----";
        }
        cout << "|\n";

        for (int c = 0; c < size; c++) {
            cout << "|  " << board[r][c] << "  ";
        }
        char rowLetter = 'A' + r;
        cout << "| " << rowLetter << "\n";
    }

    for (int c = 0; c < size; c++) {
        cout << "|-----";
    }
    cout << "|\n";

    for (int c = 1; c <= size; c++) {
        if (c < 10) {
            cout << "   " << c << "  ";
        } else {
            cout << "   " << c << " ";
        }
    }
    cout << "\n\n";
}

// دالة مخصصة لحفظ حالة اللعبة الحالية في ملف نصي خارجي (مع اختيار رقم الفتحة)
void saveGame(char** board, int size, int slot) {
    string filename = "savegame" + to_string(slot) + ".txt";
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << size << endl;
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                if (board[r][c] == ' ') {
                    outFile << '_';
                } else {
                    outFile << board[r][c];
                }
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Game saved successfully in slot " << slot << "!\n";
    } else {
        cout << "Error: Could not save the game.\n";
    }
}

// دالة مخصصة لتحميل ملف الحفظ واسترجاع اللوحة (مع اختيار رقم الفتحة)
char** loadGame(int& size, int slot) {
    string filename = "savegame" + to_string(slot) + ".txt";
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "No saved game found in slot " << slot << "!\n";
        return nullptr;
    }
    inFile >> size;
    char** board = createBoard(size);
    char ch;
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            inFile >> ch;
            if (ch == '_') {
                board[r][c] = ' ';
            } else {
                board[r][c] = ch;
            }
        }
    }
    inFile.close();
    cout << "Game loaded successfully from slot " << slot << "!\n";
    return board;
}

// دالة التحقق من صحة الحركة (Movement Rules) للقطع العادية والقدرات الخاصة
bool isValidMove(char** board, int size, int fromR, int fromC, int toR, int toC, char currentPlayer) {
    if (fromR < 0 || fromR >= size || fromC < 0 || fromC >= size || toR < 0 || toR >= size || toC < 0 || toC >= size) return false;
    if (board[toR][toC] != ' ') return false;

    char piece = board[fromR][fromC];
    if (currentPlayer == 'O' && piece != 'O' && piece != 'R' && piece != 'B' && piece != 'K') return false;
    if (currentPlayer == 'X' && piece != 'X' && piece != 'r' && piece != 'b' && piece != 'k') return false;

    int rowDiff = toR - fromR;
    int colDiff = toC - fromC;
    int absRowDiff = abs(rowDiff);
    int absColDiff = abs(colDiff);

    // 1) قواعد حركة القطع العادية (O و X)
    if (piece == 'O' || piece == 'X') {
        int forwardDirection = (piece == 'O') ? 1 : -1;
        if (rowDiff == forwardDirection && absColDiff == 1) return true;

        if (rowDiff == 2 * forwardDirection && absColDiff == 2) {
            int midR = fromR + forwardDirection;
            int midC = fromC + (colDiff / 2);
            char midPiece = board[midR][midC];
            if (currentPlayer == 'O' && (midPiece == 'X' || midPiece == 'r' || midPiece == 'b' || midPiece == 'k')) return true;
            if (currentPlayer == 'X' && (midPiece == 'O' || midPiece == 'R' || midPiece == 'B' || midPiece == 'K')) return true;
        }
        return false;
    }

    // 2) الـ ROOK (القلعة): حركة مستقيمة فقط (أفقياً أو عمودياً) بحد أقصى 3 مربعات
    if (piece == 'R' || piece == 'r') {
        if ((rowDiff == 0 && absColDiff <= 3) || (colDiff == 0 && absRowDiff <= 3)) return true;
        return false;
    }

    // 3) الـ BISHOP (الفيل): حركة قطرية (مائلة) في كل الاتجاهات بحد أقصى 4 مربعات
    if (piece == 'B' || piece == 'b') {
        if (absRowDiff == absColDiff && absRowDiff <= 4) return true;
        return false;
    }

    // 4) الـ KING (الملك المطور): مربع واحد في أي اتجاه محيط بالقطعة
    if (piece == 'K' || piece == 'k') {
        if (absRowDiff <= 1 && absColDiff <= 1) return true;
        return false;
    }

    return false;
}

// دالة التحقق من الترقية ومنح القدرات الخاصة (Special Powers) عند الوصول للطرف الآخر
void checkForPromotion(char** board, int size, int r, int c, char currentPlayer) {
    if ((currentPlayer == 'O' && r == size - 1) || (currentPlayer == 'X' && r == 0)) {
        cout << "\n============= SPECIAL POWER ELIGIBLE! =============" << endl;
        cout << "Player " << currentPlayer << ", your piece at [" << (char)('A' + r) << c + 1 << "] reached the end!" << endl;
        cout << "Select one of the 3 special powers for this piece:" << endl;
        cout << "[1] Turn into a ROOK (3 blocks vertically/horizontally)\n";
        cout << "[2] Turn into a BISHOP (4 blocks diagonally)\n";
        cout << "[3] Turn into a KING (1 block surrounding pieces)\n";
        cout << "===================================================" << endl;

        int powerChoice = 0;
        while (true) {
            cout << "Your Choice (1, 2, or 3): ";
            cin >> powerChoice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number (1, 2, or 3)." << endl;
                continue;
            }
            if (powerChoice >= 1 && powerChoice <= 3) break;
            cout << "Invalid choice! Please choose 1, 2, or 3." << endl;
        }

        if (currentPlayer == 'O') {
            if (powerChoice == 1) board[r][c] = 'R';
            else if (powerChoice == 2) board[r][c] = 'B';
            else if (powerChoice == 3) board[r][c] = 'K';
        } else if (currentPlayer == 'X') {
            if (powerChoice == 1) board[r][c] = 'r';
            else if (powerChoice == 2) board[r][c] = 'b';
            else if (powerChoice == 3) board[r][c] = 'k';
        }
        cout << "Success! Your piece has been upgraded!\n";
    }
}

// دالة تنظيف الذاكرة وحذف المصفوفة الديناميكية من الـ RAM
void deleteBoard(char** board, int size) {
    for (int i = 0; i < size; i++) delete[] board[i];
    delete[] board;
}

// دالة مساعدة لطلب رقم فتحة الحفظ (1-3) مع التحقق من الإدخال
int getSaveSlot() {
    int slot = 0;
    while (true) {
        cout << "Choose save slot (1, 2, or 3): ";
        cin >> slot;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number (1, 2, or 3)." << endl;
            continue;
        }
        if (slot >= 1 && slot <= 3) break;
        cout << "Invalid slot! Please enter 1, 2, or 3." << endl;
    }
    return slot;
}

// === دالة التحكم والتشغيل الرئيسية للمشروع ===
int main() {
    int boardSize = 0;
    char** gameBoard = nullptr;
    int menuChoice = 0;

    // شاشة الترحيب والتعليمات المدمجة مع القائمة الرقمية لسهولة الاستخدام
    cout << "welcome to Checkermania MMU made by Zul, Ghith, Rinad and Faiza \n\n";

    cout << "=========================================================\n";
    cout << "                         CHECKERMANIA                    \n";
    cout << "=========================================================\n";
    cout << " Rules & Instructions:\n";
    cout << " 1. Player 1 uses 'O' pieces (Top side).\n";
    cout << " 2. Player 2 uses 'X' pieces (Bottom side).\n";
    cout << " 3. Enter your moves using coordinates without spaces (e.g., F2).\n";
    cout << " 4. Move diagonally forward to empty boxes or jump to eliminate.\n";
    cout << " 5. Type 'SAVE' during your turn to save and quit.\n";
    cout << "=========================================================\n";
    cout << "Please select an option:\n";
    cout << "[1] NEW GAME\n";
    cout << "[2] CONTINUE (Load Game)\n";

    // حلقة التحقق من صحة اختيار القائمة مع معالجة الإدخال غير الرقمي
    while (true) {
        cout << "Your Input (1 or 2): ";
        cin >> menuChoice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number (1 or 2).\n";
            continue;
        }
        if (menuChoice == 1 || menuChoice == 2) break;
        cout << "Invalid selection! Please enter 1 or 2.\n";
    }

    // خيار تحميل اللعبة السابقة
    if (menuChoice == 2) {
        int slot = getSaveSlot();   // اطلب رقم الفتحة
        gameBoard = loadGame(boardSize, slot);
        if (gameBoard == nullptr) {
            // إذا لم يوجد ملف في هذه الفتحة، ننتقل تلقائياً لبدء لعبة جديدة
            cout << "Starting a new game instead...\n";
            menuChoice = 1;
        }
    }

    // خيار بدء لعبة جديدة واختيار الحجم
    if (menuChoice == 1) {
        while (true) {
            cout << "Enter board size (6 to 10): ";
            cin >> boardSize;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }
            if (boardSize >= 6 && boardSize <= 10) break;
            cout << "Invalid size! Please try again.\n";
        }
        gameBoard = createBoard(boardSize);
        initializeBoard(gameBoard, boardSize);
    }

    cout << "\n-- Current Board Setup (" << boardSize << "x" << boardSize << ") --\n";

    char turn = 'X'; // يبدأ اللاعب X اللعب أولاً
    string fromCoord, toCoord;

    // === حلقة اللعب الرئيسية المستمرة (Game Loop) ===
    while (true) {
        displayBoard(gameBoard, boardSize);
        cout << "Player " << turn << "'s Turn.\n";
        cout << "Enter coordinate to move FROM (or type SAVE to exit): ";
        cin >> fromCoord;

        if (fromCoord == "SAVE" || fromCoord == "save") {
            int slot = getSaveSlot();   // اختر الفتحة للحفظ
            saveGame(gameBoard, boardSize, slot);
            break;
        }

        cout << "Enter coordinate to move TO: ";
        cin >> toCoord;

        // ترجمة النصوص المدخلة (مثال: F2) إلى أرقام صفوف وأعمدة للمصفوفة
        int fromR = fromCoord[0] - 'A';
        int fromC = fromCoord[1] - '1';
        int toR = toCoord[0] - 'A';
        int toC = toCoord[1] - '1';

        // اختبار قانونية الحركة المدخلة
        if (isValidMove(gameBoard, boardSize, fromR, fromC, toR, toC, turn)) {
            char movingPiece = gameBoard[fromR][fromC];

            // تحريك القطعة على البوردة
            gameBoard[toR][toC] = movingPiece;
            gameBoard[fromR][fromC] = ' ';

            // إذا قفزت قطعة عادية خطوتين، يعني أنها أكلت قطعة الخصم، فنحذفها من المنتصف
            if ((movingPiece == 'O' || movingPiece == 'X') && abs(toR - fromR) == 2) {
                int midR = fromR + (toR - fromR) / 2;
                int midC = fromC + (toC - fromC) / 2;
                gameBoard[midR][midC] = ' ';
                cout << "Opponent piece eliminated!\n";
            }

            // فحص الترقية للقدرات الخاصة
            checkForPromotion(gameBoard, boardSize, toR, toC, turn);

            // تبديل الأدوار
            turn = (turn == 'X') ? 'O' : 'X';
        } else {
            cout << "\n!!! INVALID MOVE !!! Please follow the piece movement rules.\n";
        }
    }

    // تنظيف الذاكرة وحذف المصفوفة الديناميكية قبل الخروج تماماً من اللعبة
    deleteBoard(gameBoard, boardSize);
    return 0;
}