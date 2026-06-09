#include <iostream>
#include <string>
using namespace std;

// دالة لإنشاء الرقعة وحجز الذاكرة ديناميكياً
char** createBoard(int size) {
    char** board = new char*[size];
    for (int i = 0; i < size; i++) {
        board[i] = new char[size];
    }
    return board;
}

// دالة لتوزيع القطع الابتدائي (القطع O في الأعلى، X في الأسفل، وفراغ في الوسط)
void initializeBoard(char** board, int size) {
    int rowsOfPieces = (size == 6) ? 2 : 3; // لتحديد كم صف قطع نحتاج بناءً على الحجم

    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (r < rowsOfPieces) {
                board[r][c] = 'O'; // قطع اللاعب الأول في الأعلى
            } else if (r >= size - rowsOfPieces) {
                board[r][c] = 'X'; // قطع اللاعب الثاني في الأسفل
            } else {
                board[r][c] = ' '; // خانات فارغة في الوسط
            }
        }
    }
}


void displayBoard(char** board, int size) {
    // 1. طباعة أرقام الأعمدة بتباعد دقيق وموزون (4 مسافات لكل عمود لتوافق الفواصل)
    cout << "    "; // 4 مسافات في البداية لتغطية مساحة الأحرف والفاصل الأول
    for (int c = 1; c <= size; c++) {
        if (c < 10) {
            cout << " " << c << "  "; // رقم من خانة واحدة: مسافة قبل، الرقم، مسافتين بعد
        } else {
            cout << " " << c << " ";  // رقم 10 (خانتين): مسافة قبل، الرقم، مسافة بعد
        }
    }
    cout << "\n";

    // 2. طباعة الصفوف ومحتوياتها
    for (int r = 0; r < size; r++) {
        char rowLetter = 'A' + r;
        cout << " " << rowLetter << " |"; // طباعة الحرف والفاصل الرئيسي

        for (int c = 0; c < size; c++) {
            cout << " " << board[r][c] << " |"; // طباعة القطعة بداخل الفواصل
        }
        cout << "\n";
    }
}
// دالة تنظيف الذاكرة وحذف canالمصفوفة الديناميكية من الـ RAM
void deleteBoard(char** board, int size) {
    for (int i = 0; i < size; i++) {
        delete[] board[i];
    }
    delete[] board;
}

int main() {
    int boardSize = 0;

    cout << "== Welcome to Checkers Game, made in MMU ==\n";

    // التحقق من المدخلات: إجبار المستخدم على اختيار حجم بين 6 و 10
    while (true) {
        cout << "Enter board size (6 to 10): ";
        cin >> boardSize;

        if (boardSize >= 6 && boardSize <= 10) {
            break; // الحجم صحيح، نخرج من الحلقة
        }
        cout << "Invalid size! Please try again.\n";
    }

    // إنشاء الرقعة وتهيئتها وعرضها
    char** gameBoard = createBoard(boardSize);
    initializeBoard(gameBoard, boardSize);

    cout << "\n-- Initial Board Setup (" << boardSize << "x" << boardSize << ") ---\n";
    displayBoard(gameBoard, boardSize);

    // تنظيف الذاكرة قبل الخروج
    deleteBoard(gameBoard, boardSize);

    return 0;
}
