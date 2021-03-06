#include<iostream>
#include<string>
#include<list>
#include<stdio.h>
#include<thread>
using namespace std;
#pragma warning(disable:4996)

/*しりとりの内容と数を報告する*/
struct sRslt {
	string text;
	int count;
};
list<sRslt>RsltList;//ここに報告を蓄積する
void showRslt() {
	/*内容が重複するもの及び数が0か1のもの削除する*/
	cout << "◆結果を整理整頓中　全"<<RsltList.size()<<"項目"<<endl;
	list<sRslt>::iterator coma, comb;
	coma = RsltList.begin();
	while (coma != RsltList.end()) {
		if (coma->count == 0 || coma->count == 1) {
			RsltList.erase(coma);
			coma = RsltList.begin();
			continue;
		}
		++coma;
	}
	cout << RsltList.size() << "項目まで絞れた" << endl;
	coma = RsltList.begin();
	
	while(coma!=RsltList.end()){
		comb = RsltList.begin();
		while (comb != RsltList.end()) {
			if (coma != comb
				&& coma->text == comb->text) {
				RsltList.erase(comb);
				comb = RsltList.begin();
				coma = RsltList.begin();
				continue;
			}
			else { ++comb; }
		}
		++coma;
	}

	/*結果発表*/
	cout << "◆今から結果発表です" << endl;
	list<sRslt>::iterator rsltitr;
	for (int num = 0;; num++) {
		if (RsltList.size() == 0)break;
		cout << num << "個繋げられた奴～～" << endl;
		rsltitr = RsltList.begin();
		while (rsltitr != RsltList.end()) {
			if (rsltitr->count == num) {
				cout <<"　　"<< rsltitr->text << endl;
				rsltitr = RsltList.erase(rsltitr);
			}
			else {
				++rsltitr;
			}
		}
	}
	cout << "以上です" << endl;
	cout << "Press key to proceed" << endl, getchar();
}

/*単語の構造体*/
struct sWord {
	string text;
	bool available;
	list <list<sWord>::iterator> aftrindx;//この単語の後に続く単語へのイテレータを蓄積する
};
list<sWord> WordList;//ここに単語を蓄積する

/*ファイルの読み込み*/
void Load() {cout << "◆ファイルを読み込みます" << endl;

	FILE *fh = fopen("WordList.txt", "r");
	char buf[256];
	while (fscanf(fh, "%s\n", buf) != EOF) {
		WordList.push_back(sWord{ buf,true, });
	}
	fclose(fh);
}

/*インデックス作成*/
void makeIndx() {cout << "◆インデックスを作成します" << endl;

	list<sWord>::iterator me, it;
	me = WordList.begin();
	while (me != WordList.end()) {
		it = WordList.begin();
		while (it != WordList.end()) {
			if (me->text.substr(me->text.length() - 2, 2)//meの末尾が
				== it->text.substr(0, 2)//itの先頭と同じであり
				&& me!= it) {//両者が違う者同士であった
					me->aftrindx.push_back(it);
			}
			++it;
		}
		++me;
	}
}
void showIndx() {cout << "◆インデックス作成が終了したので一覧を表示します" << endl;

	list<sWord>::iterator me;
	list <list<sWord>::iterator> ::iterator myafter;

	me = WordList.begin();
	while (me != WordList.end()) {
		cout << "　　"<< me->text << "→ ";
		myafter = me->aftrindx.begin();
		while (myafter != me->aftrindx.end()) {
			cout << (*myafter)->text << " , ";
			++myafter;
		}
		cout << endl;
		++me;
	}

	cout << "Press key to proceed" << endl, getchar();
}

/*しりとりを作成する*/
void makeShiritori(list<sWord>::iterator me,string rslt, int count) {

	cout << rslt << endl;//結果報告
	thread notify([&] {
		RsltList.push_back(sRslt{ rslt,count });//結果報告
	});

	me->available = false;//ロックする

	list <list<sWord>::iterator>::iterator myafter=me->aftrindx.begin();
	while (myafter != me->aftrindx.end()) {
		if ((*myafter)->available) {
			makeShiritori(
				*myafter, 
				rslt + " → " + (*myafter)->text,
				count + 1);
		}
		++myafter;
	}

	me->available = true;//ロック解除
	notify.join();
}

int main()
{
	Load();
	makeIndx();
	showIndx();

	list<sWord>::iterator first = WordList.begin();
	while (first != WordList.end()) {
		makeShiritori(first, first->text, 1);
		++first;
	}

	showRslt();
    return 0;
}