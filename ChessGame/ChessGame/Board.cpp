#include "Board.h"
Board::Board() {//�ѽL�w�]��l��
	init();
}
void Board::recycle() {
	for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				delete m[i][j];
			}
	}
}
//Board::~Board() {
//	for (int i = 0; i < 8; ++i) {
//		for (int j = 0; j < 8; ++j) {
//			delete m[i][j];
//		}
//	}
//}
int Board::pieceNum() {
	int count = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (m[i][j] != NULL) {
				++count;
			}
		}
	}
	return count;
}
void Board::saveReplay() {
	std::ofstream ofs(name);
	auto it = stepRecord.begin();
	while (it != stepRecord.end()) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				ofs << (*it)[i][j].first;
				ofs << (*it)[i][j].second;
			}
		}
		it++;
	}
}
void Board::recreate(vector<vector<pair<int,int>>> v) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (v[i][j].first == Tab::empty) {
				m[i][j] = NULL;
			}
			else {
				m[i][j] = createPiece(v[i][j].first);
				m[i][j]->owner = v[i][j].second;
				m[i][j]->pos = Position(i,j);
			}
		}
	}
}
void Board::record() {
	vector<vector<pair<int, int>>> rec;
	rec.resize(8);
	for (int i = 0; i < 8; ++i)
		rec[i].resize(8);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (m[i][j] != NULL) {
				rec[i][j].first = m[i][j]->getClass();
				rec[i][j].second = m[i][j]->owner;
			}
			else {
				rec[i][j].first = Tab::empty;
				rec[i][j].second = 3;
			}
		}
	}
	stepRecord.push_back(rec);
}
bool Board::isVaild(Position p,int player,bool kusing) {
	if (player == 1) {//�令�ˬd���Ѥl
		player = 2;
	}
	else if (player == 2) {
		player = 1;
	}
	
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (m[i][j] != NULL && m[i][j]->owner == player) {
				if (kusing && m[i][j]->getClass() == Tab::king) {
					continue;
				}
				vector<Position> tmp = m[i][j]->possibleMove(*this,Position(i,j));
				for (Position ppp : tmp) {
					if (ppp == p)
						return false;
				}
			}
		}
	}
	return true;
}
void Board::copyBoard(Board b) {
	steps = b.steps;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (b.m[i][j] != NULL) {
				m[i][j] = createPiece(b.m[i][j]->getClass());
				m[i][j]->pos = b.m[i][j]->pos;
				m[i][j]->owner = b.m[i][j]->owner;
			}
			else {
				m[i][j] = NULL;
			}
			
		}
	}
}

vector<Position> Piece::possibleMove(Board& m, Position P) {
	return vector<Position>();
}
int Piece::getClass() {
	return Tab::empty;
}
void Piece::change(Board& M, Position P) {
	return;
}



Piece* Board::createPiece(int kind) {

	switch (kind)
	{
	case pawn:
		return new Pawn();
	case rock:
		return new Rock();
	case knight:
		return new Knight();
	case bishop:
		return new Bishop();
	case queen:
		return new Queen();
	case king:
		return new King();
	default:
		return NULL;
	}
}
void Board::save() {
	string file = name;
	file += "save";
	std::ofstream ofs(file);

		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (m[i][j] != NULL) {
					ofs << m[i][j]->getClass();
					ofs << m[i][j]->owner;
				}
				else {
					ofs << Tab::empty;
					ofs << "3";
				}
				
			}
		}
	
	ofs << " ";
	for (pair<Position,Position> p : steps) {
		ofs << p.first.y << " " << p.first.x << " " << p.second.y << " " << p.second.x << " ";
	}
}

void Board::init() {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	name = "" + std::to_string(ltm->tm_mon) + std::to_string(ltm->tm_mday) + std::to_string(ltm->tm_hour) + std::to_string(ltm->tm_min)
		+ std::to_string(ltm->tm_sec);


	peaceMove = 0;
	someoneSurrender = false;
	callbackMenu = false;


	int initFirstline[8] = { rock,knight,bishop,queen,king,bishop,knight,rock };
	int initSecondline[8] = { pawn ,pawn ,pawn ,pawn ,pawn ,pawn ,pawn ,pawn };
	
	/*int initFirstline[8] = { empty,empty,bishop,empty,king,empty,empty,empty };
	int initSecondline[8] = { pawn ,empty ,empty ,empty ,empty ,empty ,empty ,empty };*/
	
	m.resize(8);
	for (int i = 0; i < 8; ++i) {	//�M�ŴѽL
		m[i].resize(8);
		for (int j = 0; j < 8; ++j) {
			m[i][j] = NULL;
		}
	}
	for (int i = 0; i < 8; ++i) {	//�]�w�w�]�ѽL
		if (initFirstline[i] == empty) {
			continue;
		}
		m[0][i] = createPiece(initFirstline[i]);
		m[0][i]->owner = 2;
		m[0][i]->pos = Position(0, i);

		
		m[7][i] = createPiece(initFirstline[i]);
		m[7][i]->owner = 1;
		m[7][i]->pos = Position(7, i);

	}
	for (int i = 0; i < 8; ++i) {	//�]�w�w�]�ѽL
		if (initSecondline[i] == empty) {
			continue;
		}
		m[1][i] = createPiece(initSecondline[i]);
		m[1][i]->owner = 2;
		m[1][i]->pos = Position(1, i);

		m[6][i] = createPiece(initSecondline[i]);
		m[6][i]->owner = 1;
		m[6][i]->pos = Position(6, i);
	}

	stepRecord.clear();
	steps.clear();
	record();//�����_�l�ѽL
		
}
vector<Position> Pawn::possibleMove( Board& m,Position pos) {
	Position forward, killL, killR;//
	vector<Position> p;//�����i����m
	int opponentFirstTwoStepLine = -1;//��⪺�Ĥ@�B�����pawn���I��y
	int startLine = -1;//��y��m���ˬd�O�_���Ĥ@�B
	if (owner == 1) {//�U��զ⩹�W��
		startLine = 6;
		opponentFirstTwoStepLine = 3;
		forward = Position(-1, 0);
		killL = Position(-1, -1);
		killR = Position(-1, 1);
	}
	else if (owner == 2) {//�W��¦⩹�U��
		startLine = 1;
		opponentFirstTwoStepLine = 4;
		forward = Position(1, 0);
		killL = Position(1, -1);
		killR = Position(1, 1);
	}
	if (pos.y == startLine) {	//�Ĥ@�B�����
		Position oneStep = pos + forward;
		Position tmp = forward + forward;
		tmp = pos + tmp;
		if (tmp.inRange()&&m.m[oneStep.y][oneStep.x]==NULL&&m.m[tmp.y][tmp.x] == NULL) {
			p.push_back(tmp);
		}
	}

	Position tmp = pos + forward;//Pawn���e���@��
	if (tmp.inRange()) {//���W��
		if (m.m[tmp.y][tmp.x] == NULL) {
			p.push_back(tmp);
		}
	}
	tmp = pos + killL;//Pawn�Y���e
	if (tmp.inRange()) {//���W��
		if (m.m[tmp.y][tmp.x] != NULL && m.m[tmp.y][tmp.x]->owner != this->owner) {//���O�ۤv���Ѥl�B���Ѥl
			p.push_back(tmp);
		}
	}
	tmp = pos + killR;//Pawn�Y�k�e
	if (tmp.inRange()) {//���W��
		if (m.m[tmp.y][tmp.x] != NULL && m.m[tmp.y][tmp.x]->owner != this->owner) {//���O�ۤv���Ѥl�B���Ѥl
			p.push_back(tmp);
		}
	}
	tmp = pos + Position(0, -1);//�ˬd����ӱo���O�_���L���L
	if (tmp.inRange()&&tmp.y==opponentFirstTwoStepLine) {//�ˬd�O�_���W�ɥB�O���i�ਫ��B��y
		if (m.m[tmp.y][tmp.x]!=NULL&&m.m[tmp.y][tmp.x]->getClass() == Tab::pawn) {//�ˬd�O�_��pawn
			Position pTmp = tmp + forward + forward;//��⪺��h�O�ۤv���e�i
			if (pTmp == m.steps.at(m.steps.size() - 1).first) {//�ˬd�W�@�B�O����B
				p.push_back(tmp + forward);
			}
		}
	}
	tmp = pos + Position(0, 1);//�ˬd�k��ӱo���O�_���L���L
	if (tmp.inRange() && tmp.y == opponentFirstTwoStepLine) {//�ˬd�O�_���W�ɥB�O���i�ਫ��B��y
		if (m.m[tmp.y][tmp.x] != NULL && m.m[tmp.y][tmp.x]->getClass() == Tab::pawn) {//�ˬd�O�_��pawn
			Position pTmp = tmp + forward + forward;//��⪺��h�O�ۤv���e�i
			if (pTmp == m.steps.at(m.steps.size() - 1).first) {//�ˬd�W�@�B�O����B
				p.push_back(tmp + forward);
			}
		}
	}
	return p;
}
vector<Position> Rock::possibleMove( Board& m, Position pos) {
	vector<Position> move;
	move.push_back(Position(1, 0));
	move.push_back(Position(-1, 0));
	move.push_back(Position(0, 1));
	move.push_back(Position(0, -1));
	vector<Position> p;
	Position tmp;
	for (Position t : move) {
		tmp = pos;
		while (true) {
			tmp = tmp + t;
			if (tmp.inRange()) {
				if (m.m[tmp.y][tmp.x] == NULL) {
					p.push_back(tmp);
					continue;
				}
				else {
					if (m.m[tmp.y][tmp.x]->owner != this->owner) {
						p.push_back(tmp);
						break;
					}
				}
			}
			break;
		}
	}
	return p;
}

vector<Position> Knight::possibleMove( Board& m, Position pos) {
	vector<Position> move;
	move.push_back(Position(-2,-1));
	move.push_back(Position(-1, -2));
	move.push_back(Position(-2, 1));
	move.push_back(Position(-1, 2));
	move.push_back(Position(2, -1));
	move.push_back(Position(1, -2));
	move.push_back(Position(2, 1));
	move.push_back(Position(1, 2));
	vector<Position> p;
	for (Position t: move) {
		Position tmp = pos + t;
		if (tmp.inRange()) {//���W��
			if (m.m[tmp.y][tmp.x] != NULL && m.m[tmp.y][tmp.x]->owner != this->owner) {//���O�ۤv���Ѥl�B���Ѥl
				p.push_back(tmp);
			}
			else if (m.m[tmp.y][tmp.x] == NULL) {
				p.push_back(tmp);
			}
		}
	}
	return p;
}
vector<Position> Bishop::possibleMove( Board& m, Position pos) {
	vector<Position> p;
	vector<Position> move;
	move.push_back(Position(-1, -1));
	move.push_back(Position(-1, 1));
	move.push_back(Position(1, -1));
	move.push_back(Position(1, 1));
	Position tmp;
	for (Position t : move) {
		tmp = pos;
		while (true) {
			tmp = tmp + t;
			if (tmp.inRange()) {
				if (m.m[tmp.y][tmp.x] == NULL) {
					p.push_back(tmp);
					continue;
				}
				else {
					if (m.m[tmp.y][tmp.x]->owner != this->owner) {
						p.push_back(tmp);
						break;;
					}
				}
			}
			break;
		}
	}
	return p;
	
}
vector<Position> Queen::possibleMove( Board& m, Position pos) {
	vector<Position> move;
	move.push_back(Position(1, 0));
	move.push_back(Position(-1, 0));
	move.push_back(Position(0, 1));
	move.push_back(Position(0, -1));
	move.push_back(Position(-1, -1));
	move.push_back(Position(-1, 1));
	move.push_back(Position(1, -1));
	move.push_back(Position(1, 1));
	vector<Position> p;
	Position tmp;
	for (Position t : move) {
		tmp = pos;
		while (true) {
			tmp = tmp + t;
			if (tmp.inRange()) {
				if (m.m[tmp.y][tmp.x] == NULL) {
					p.push_back(tmp);
					continue;
				}
				else {
					if (m.m[tmp.y][tmp.x]->owner != this->owner) {
						p.push_back(tmp);
						break;;
					}
				}
			}
			break;
		}
	}
	return p;
}

vector<Position> King::possibleMove( Board& m, Position pos) {
	vector<Position> move;
	move.push_back(Position(-1, -1));
	move.push_back(Position(0, -1));
	move.push_back(Position(1, -1));
	move.push_back(Position(-1, 0));
	move.push_back(Position(1, 0));
	move.push_back(Position(-1, 1));
	move.push_back(Position(0, 1));
	move.push_back(Position(1, 1));
	vector<Position> p;
	for (Position t : move) {
		Position tmp = pos + t;
		if (tmp.inRange()) {//���W��
			if (m.m[tmp.y][tmp.x] == NULL) {
				p.push_back(tmp);
			}
			else if (m.m[tmp.y][tmp.x] != NULL && m.m[tmp.y][tmp.x]->owner != this->owner) {//���O�ۤv���Ѥl�B���Ѥl
				p.push_back(tmp);
			}
		}
	}
	if (m.current_player == owner&& m.isVaild(this->pos, owner, true)) {
		//���������ˬd
		bool LnotMoved = true;
		bool RnotMoved = true;
		Position kMove, lCMove, rCMove; //�]�m�w�]�Ѥl��m �q���ʬ����ˬd�O�_�����ʹL
		if (owner == 1) {//�U��զ�
			kMove = Position(7, 4);
			lCMove = Position(7, 0);
			rCMove = Position(7, 7);
		}
		else if (owner == 2) {//�W��¦�
			kMove = Position(0, 4);
			lCMove = Position(0, 0);
			rCMove = Position(0, 7);
		}
		for (pair<Position, Position>p : m.steps) {
			if (p.first == kMove) {//����Q�ʹL
				RnotMoved = false;
				LnotMoved = false;
				break;
			}
			if (p.first == lCMove) {//���䦳�Q���ʹL
				LnotMoved = false;
			}
			if (p.first == rCMove) {//���䦳�Q���ʹL
				RnotMoved = false;
			}
		}
		if (LnotMoved) {
			bool possibleStat = true;//���������W�O�_���
			for (int i = 1; i < 4; ++i) {//�ˬd�������ण�ഫ
				Position tmp(pos.y, i);
				if ((m.m[tmp.y][tmp.x] != NULL) || (!m.isVaild(tmp, owner, true))) {
					possibleStat = false;
					break;
				}
			}
			if (possibleStat) {
				p.push_back(lCMove);
			}
		}
		if (RnotMoved) {
			bool possibleStat = true;//���������W�O�_���
			for (int i = 5; i < 7; ++i) {//�ˬd�k�����ण�ഫ
				Position tmp(pos.y, i);
				if ((m.m[tmp.y][tmp.x] != NULL) || (!m.isVaild(tmp, owner, true))) {
					possibleStat = false;
					break;
				}
			}
			if (possibleStat) {
				p.push_back(rCMove);
			}
		}
	}
	

	return p;
}
int Pawn::getClass() {
	return pawn;
}
int Rock::getClass() {
	return rock;
}
int Knight::getClass() {
	return knight;
}
int Bishop::getClass() {
	return bishop;
}
int Queen::getClass() {
	return queen;
}
int King::getClass() {
	return king;
}

void Pawn::change(Board& m, Position B) {
	Position  backward,tmp;
	if (pos.x != B.x) {//�O�ר�
		if (owner == 1) {//�U��զ⩹�W��	//�ˬd�L���L��possibleMove�t�d �p�G�Y�׫e�ɦY��ťեN���L���L
			backward = Position(1, 0);
			if (m.m[B.y][B.x] == NULL) {
				tmp = B + backward;
				m.m[tmp.y][tmp.x] = NULL;
			}
		}
		else if (owner == 2) {//�W��¦⩹�U��
			backward = Position(-1, 0);
			if (m.m[B.y][B.x] == NULL) {
				tmp = B + backward;
				m.m[tmp.y][tmp.x] = NULL;
			}
		}
	}
	

	m.m[B.y][B.x] = m.m[pos.y][pos.x];
	m.m[pos.y][pos.x] = NULL;
	pos.y = B.y;
	pos.x = B.x;
}
void Rock::change(Board& m, Position B) {
	m.m[B.y][B.x] = m.m[pos.y][pos.x];
	m.m[pos.y][pos.x] = NULL;
	pos.y = B.y;
	pos.x = B.x;
}
void Knight::change(Board& m,  Position B) {
	m.m[B.y][B.x] = m.m[pos.y][pos.x];
	m.m[pos.y][pos.x] = NULL;
	pos.y = B.y;
	pos.x = B.x;
}
void Bishop::change(Board& m, Position B) {
	m.m[B.y][B.x] = m.m[pos.y][pos.x];
	m.m[pos.y][pos.x] = NULL;
	pos.y = B.y;
	pos.x = B.x;
}
void Queen::change(Board& m, Position B) {
	m.m[B.y][B.x] = m.m[pos.y][pos.x];
	m.m[pos.y][pos.x] = NULL;
	pos.y = B.y;
	pos.x = B.x;
}
void King::change(Board& m, Position B) {
	if (m.m[B.y][B.x]!=NULL&&m.m[B.y][B.x]->getClass() == Tab::rock&&m.m[B.y][B.x]->owner==owner) {//��������
		if (B.x == 0) {
			m.m[B.y][B.x + 3] = m.m[B.y][B.x];
			m.m[B.y][B.x]->pos = Position(B.y, B.x + 3);
			m.m[B.y][B.x] = NULL;
			
			m.m[pos.y][pos.x - 2] = m.m[pos.y][pos.x];
			m.m[pos.y][pos.x] = NULL;
			pos.x = pos.x - 2;
		}
		else if (B.x == 7) {
			m.m[B.y][B.x - 2] = m.m[B.y][B.x];
			m.m[B.y][B.x]->pos = Position(B.y, B.x - 2);
			m.m[B.y][B.x] = NULL;

			m.m[pos.y][pos.x + 2] = m.m[pos.y][pos.x];
			m.m[pos.y][pos.x] = NULL;
			pos.x = pos.x +2;
		}
	}
	else {
		m.m[B.y][B.x] = m.m[pos.y][pos.x];
		m.m[pos.y][pos.x] = NULL;
		pos.y = B.y;
		pos.x = B.x;
	}
	
}


Position::Position() {
	
}
Position::Position(int y,int x) {
	this->x = x;
	this->y = y;
}
Position Position::operator+(Position p) {
	return Position(y + p.y, x + p.x);
}
bool Position::operator==(Position p) {
	if (this->x == p.x && this->y == p.y)
		return true;
	return false;
}
bool Position::operator!=(Position p) {
	if (this->x == p.x && this->y == p.y)
		return false;
	return true;
}

bool Position::inRange() {
	if (x < 8 && y < 8 && x >= 0 && y >= 0) {
		return true;
	}
	return false;
}
