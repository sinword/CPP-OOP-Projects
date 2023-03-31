#include "Player.h"
int Viewer::fontW = 0;
int Viewer::fontH = 0;
Position getClick() {
    Position pos;
    while (true) {
        POINT p;
        if (GetAsyncKeyState(VK_LBUTTON)) {
            GetCursorPos(&p);
            ScreenToClient(GetForegroundWindow(), &p);
            pos.y = p.y / Viewer::fontH;
            pos.x = p.x / Viewer::fontW;
            break;
        }
    }
    return pos;
}
Player::Player(int i) {
    if (i == 1) {
        kPos = Position(7, 4);
    }
    else if (i == 2) {
        kPos = Position(0, 4);
    }
}

HumanPlayer::HumanPlayer(int i) :Player(i){
    count_T = 900;
    owner = i;
}
basicAiPlayer::basicAiPlayer(int i):Player(i) {
    owner = i;
    srand(time(NULL));
}
void HumanPlayer::OnMove(Board& b, Viewer& v)
{
    bool dofunc = false;
    bool surrend = false;
    bool backtoMenu = false;
    int prePieceNum = b.pieceNum();//移動前棋子數
    b.current_player = owner;
    //抓取位置並印出可走位置
    vector<Position> ppp;
    Position preChoose;//上次選取位置
    Position tablePos;//選取的位置

    while (true) {
        if (!b.isVaild(kPos, owner, false)) {
            v.warn(kPos, 153);
        }
        bool moving = false;//檢查是否選取可走位置
        Position pos;
        int tableY;
        int tableX;
        while (true) {
            pos = getClick();
            tableY = pos.y / 7;
            tableX = pos.x / 13;
            if (tableX == 8) {
                if (tableY == 0) {
                    dofunc = true;
                    int tmpSize = b.stepRecord.size();
                    if (tmpSize-3 < 0) {
                        v.showMessage("Can't undo",true,0);
                        break;
                    }
                    b.redoSteps = b.steps;
                    b.steps.pop_back();
                    b.steps.pop_back();
                    b.redoStepRecord = b.stepRecord;
                    b.recreate(b.stepRecord[b.stepRecord.size() - 3]);
                    b.stepRecord.pop_back();
                    b.stepRecord.pop_back();
                    v.setTable(b);
                    v.draw();
                    break;
                }
                else if (tableY == 1) {
                    dofunc = true;
                    if (b.redoStepRecord.empty()) {
                        v.showMessage("Can't redo", true, 0);
                        break;
                    }
                    b.recreate(b.redoStepRecord[b.redoStepRecord.size()-1]);
                    b.stepRecord = b.redoStepRecord;
                    b.steps = b.redoSteps;
                    b.redoStepRecord.clear();
                    b.redoSteps.clear();
                    v.setTable(b);
                    v.draw();
                    break;
                }
                else if (tableY == 2) {
                    surrend = true;
                    if (owner == 1) {
                        v.showMessage("White surrender", false, 5000);
                    }
                    else if (owner == 2) {
                        v.showMessage("Black surrender", false, 5000);
                    }
                    b.someoneSurrender = true;
                    break;
                }
                else if (tableY == 3) {
                    b.save();
                    v.showMessage("Save Success", false, 1000);
                }
                else if (tableY == 4) {
                    backtoMenu = true;
                    break;
                }
            }
            if (tableY < 8 && tableY >= 0 && tableX < 8 && tableX >= 0) {//需要檢查各個range 避免點到外面時出錯
                tablePos.x = tableX;
                tablePos.y = tableY;
                for (Position movePos : ppp) {
                    if (movePos.x == tablePos.x&&movePos.y==tablePos.y) {
                        Position kingposition_tmp= kPos;//臨時國王位置 因為可能移動者為國王本身
                        Board tmpBoard;
                        tmpBoard.copyBoard(b);
                        if (tmpBoard.m[preChoose.y][preChoose.x]->getClass() == Tab::king) {
                            kingposition_tmp = tablePos;
                            if (tmpBoard.m[tablePos.y][tablePos.x]!=NULL&&tmpBoard.m[tablePos.y][tablePos.x]->getClass() == Tab::rock) {
                                if (tablePos.x == 7) {
                                    kingposition_tmp.x -= 1;
                                }
                                else if (tablePos.x == 0) {
                                    kingposition_tmp.x += 2;
                                }
                            }
                        }
                        tmpBoard.m[preChoose.y][preChoose.x]->change(tmpBoard, tablePos);
                        if (tmpBoard.isVaild(kingposition_tmp, owner, false)) {
                            moving = true;
                            tmpBoard.recycle();
                            break;
                        }
                        v.warn(tablePos,204);
                        Sleep(100);
                        v.warn(tablePos, 238);
                        v.showMessage("You have been CheckMate", false, 100);
                        tmpBoard.recycle();
                    }
                }
                
                if (moving) {
                    break;
                }
                if (b.m[tableY][tableX] == NULL) {                      //需檢查點擊無棋子位置時忽略
                    continue;
                }
                if (b.m[tableY][tableX]->owner != this->owner) {//檢查選取棋子是不是自己的
                    continue;
                }
                break;

            }
        }
        if (backtoMenu) {
            break;
        }
        if (surrend) {
            surrend = false;
            break;
        }
        if (dofunc) {
            dofunc = false;
            continue;
        }
        if (moving) {
            break;
        }
        preChoose = tablePos;
        ppp = b.m[tableY][tableX]->possibleMove(b, tablePos);
        v.setPmove(ppp);
        v.draw();
    }
    if (backtoMenu) {
        backtoMenu = false;
        b.callbackMenu = true;
        return;
    }
    if (b.someoneSurrender) {
        return;
    }
    //處理移動
    if (b.m[preChoose.y][preChoose.x]->getClass() == Tab::king) {
        kPos = tablePos;
    }
    b.m[preChoose.y][preChoose.x]->change(b, tablePos);
    b.steps.push_back(pair<Position,Position>(preChoose, tablePos));
    int postPieceNum = b.pieceNum();//移動後棋子數
    if (prePieceNum == postPieceNum) {
        ++b.peaceMove;
    }else if(prePieceNum != postPieceNum) {
        b.peaceMove = 0;
    }


    v.setTable(b);
    v.clrPmove();
    v.draw();
}

void HumanPlayer::OnPromote(Board& b, Viewer& v) {
    int searchLine;//尋找有沒有pawn的y
    if (owner == 1) {
        searchLine = 0;
    }
    else if (owner == 2) {
        searchLine = 7;
    }
    for (int i = 0; i < 8; ++i) {
        if (b.m[searchLine][i] != NULL && b.m[searchLine][i]->getClass() == Tab::pawn) {//找pawn
            int pieceClass;
            v.promoteMenu();
            Position pos;
            int tableY, tableX;
            while (true) {
                pos = getClick();
                tableY = pos.y;
                tableX = pos.x;
                if (tableY <= 13 && tableY >= 5 && tableX <= 102 && tableX >= 51) {//需要檢查各個range 避免點到外面時出錯
                    pieceClass = (tableX - 51)/13 + 1;
                    break;
                }
            }
            b.m[searchLine][i] = b.createPiece(pieceClass);
            b.m[searchLine][i]->pos = Position(searchLine, i);
            b.m[searchLine][i]->owner = owner;
            clearS();
            v.setTable(b);
            v.clrPmove();
            v.draw();
        }
    }
}
bool HumanPlayer::tieCheck(Board& b, Viewer& v) {
    if (b.peaceMove == 50) {
        return true;
    }
    return false;
}

int HumanPlayer::lose(Board& b, Viewer& v) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b.m[i][j] != NULL && b.m[i][j]->owner == owner) {
                vector<Position> ppp = b.m[i][j]->possibleMove(b,Position(i,j));
                for (Position movePos : ppp) {
                        Position kingposition_tmp = kPos;//臨時國王位置 因為可能移動者為國王本身
                        Board tmpBoard;
                        tmpBoard.copyBoard(b);
                        if (tmpBoard.m[i][j]->getClass() == Tab::king) {
                            kingposition_tmp = movePos;
                        }
                        tmpBoard.m[i][j]->change(tmpBoard, movePos);   
                        if (tmpBoard.isVaild(kingposition_tmp, owner, false)) {
                            tmpBoard.recycle();
                            return 0;
                        }
                        tmpBoard.recycle();
                }
            }
        }
    }
    if (b.isVaild(kPos,owner, false)) {
        return 2;//無子可動和局
    }
    return 1;
}




//basicAiPlayer 函數
vector<pair<Position, Position>> basicAiPlayer::moveableCombine(Board& b, Viewer& v) {
    vector<pair<Position, Position>> possibleCombine;
    Position moveFrom;
    Position moveTo;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            moveFrom = Position(i, j);
            if (b.m[i][j] != NULL && b.m[i][j]->owner == owner) {
                vector<Position> ppp = b.m[i][j]->possibleMove(b, Position(i, j));
                for (Position movePos : ppp) {
                    moveTo = movePos;
                    Position kingposition_tmp = kPos;//臨時國王位置 因為可能移動者為國王本身
                    Board tmpBoard;
                    tmpBoard.copyBoard(b);
                    if (tmpBoard.m[i][j]->getClass() == Tab::king) {
                        kingposition_tmp = movePos;
                    }
                    tmpBoard.m[i][j]->change(tmpBoard, movePos);
                    if (tmpBoard.isVaild(kingposition_tmp, owner, false)) {
                        possibleCombine.push_back(pair<Position, Position>(moveFrom, moveTo));
                    }
                    tmpBoard.recycle();
                }
            }
        }
    }
    return possibleCombine;
}
void basicAiPlayer::OnMove(Board& b, Viewer& v)
{
    int prePieceNum = b.pieceNum();//移動前棋子數
    b.current_player = owner;
    

    vector<pair<Position, Position>> possibleCombine = moveableCombine(b,v);
    int randmove = rand() % possibleCombine.size();
    Position moveFrom;//從
    Position moveTo;//到
    moveFrom = possibleCombine[randmove].first;
    moveTo = possibleCombine[randmove].second;


    //處理移動
    if (b.m[moveFrom.y][moveFrom.x]->getClass() == Tab::king) {
        kPos = moveTo;
    }
    b.m[moveFrom.y][moveFrom.x]->change(b, moveTo);
    b.steps.push_back(pair<Position, Position>(moveFrom, moveTo));
    int postPieceNum = b.pieceNum();//移動後棋子數
    if (prePieceNum == postPieceNum) {
        ++b.peaceMove;
    }
    else if (prePieceNum != postPieceNum) {
        b.peaceMove = 0;
    }


    v.setTable(b);
    v.clrPmove();
    v.draw();
}

void basicAiPlayer::OnPromote(Board& b, Viewer& v) {
    int searchLine;//尋找有沒有pawn的y
    if (owner == 1) {
        searchLine = 0;
    }
    else if (owner == 2) {
        searchLine = 7;
    }
    for (int i = 0; i < 8; ++i) {
        if (b.m[searchLine][i] != NULL && b.m[searchLine][i]->getClass() == Tab::pawn) {//找pawn
            int pieceClass;
            v.promoteMenu();
            Position pos;
            int tableY, tableX;
            while (true) {
                pos = getClick();
                tableY = pos.y;
                tableX = pos.x;
                if (tableY <= 13 && tableY >= 5 && tableX <= 102 && tableX >= 51) {//需要檢查各個range 避免點到外面時出錯
                    pieceClass = (tableX - 51) / 13 + 1;
                    break;
                }
            }
            b.m[searchLine][i] = b.createPiece(pieceClass);
            b.m[searchLine][i]->pos = Position(searchLine, i);
            b.m[searchLine][i]->owner = owner;
            clearS();
            v.setTable(b);
            v.clrPmove();
            v.draw();
        }
    }
}
bool basicAiPlayer::tieCheck(Board& b, Viewer& v) {
    if (b.peaceMove == 50) {
        return true;
    }
    return false;
}

int basicAiPlayer::lose(Board& b, Viewer& v) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b.m[i][j] != NULL && b.m[i][j]->owner == owner) {
                vector<Position> ppp = b.m[i][j]->possibleMove(b, Position(i, j));
                for (Position movePos : ppp) {
                    Position kingposition_tmp = kPos;//臨時國王位置 因為可能移動者為國王本身
                    Board tmpBoard;
                    tmpBoard.copyBoard(b);
                    if (tmpBoard.m[i][j]->getClass() == Tab::king) {
                        kingposition_tmp = movePos;
                    }
                    tmpBoard.m[i][j]->change(tmpBoard, movePos);
                    if (tmpBoard.isVaild(kingposition_tmp, owner, false)) {
                        tmpBoard.recycle();
                        return 0;
                    }
                    tmpBoard.recycle();
                }
            }
        }
    }
    if (b.isVaild(kPos, owner, false)) {
        return 2;//無子可動和局
    }
    return 1;
}
