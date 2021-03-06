#include<string>
#include<iostream>
#include<list>
#include<stdio.h>
using namespace std;
#pragma warning(disable:4996)

struct Ranking {
	string text;
	int count;
};
list<Ranking> ranklist;
list<Ranking>::iterator rankitr;

struct WordList {
	string word;
	bool isAvailable;
};

//今の状態の単語リスト、最後の文字
void Shiritori(list<WordList> wl, string prev, string all,int count) {
	list<WordList>::iterator iwl;
	iwl = wl.begin();
	while (iwl != wl.end()) {
		if (iwl->isAvailable) {//その単語がまだ使われていなく
			if ((iwl->word).substr(0, 2) == prev || all=="") {//その単語の最初の文字が前の単語の最後と同じ or 初発
				iwl->isAvailable = false;//この単語を使用中とし
				cout << all << " ("<<count<<")"<< endl;
				ranklist.push_back(Ranking{all, count});
				Shiritori(//次の思考へ
					wl, 
					(iwl->word).substr(iwl->word.length() - 2, 2),
					(all + " →　" + iwl->word),
					count+1
				);
			}
			iwl->isAvailable = true;
		}
		++iwl;
	}
}

int main()
{
	/*ファイルの読み込み*/
	FILE *fh = fopen("WordList.txt", "r");
	list<WordList>defaultwl;
	char buf[256];
	while (fscanf(fh, "%s\n", buf) != EOF) {
		WordList insert;
		insert.word = buf, insert.isAvailable = true;
		defaultwl.push_back(insert);
	}
	fclose(fh);

	/*しりとり*/
	Shiritori(defaultwl, "", "",0);

	/*ランクリストの結果発表*/

	//リストの重複削除と単語数0と1の排除
	cout << "整理整頓中";
	{list<Ranking>::iterator coma, comb;
	coma = ranklist.begin();
	while (coma != ranklist.end()) {
		if (coma->count <= 1)ranklist.erase(coma), coma = ranklist.begin();//単語数0と1の排除
		comb = ranklist.begin();
		while (comb != ranklist.end()) {
			if (coma!=comb && coma->text == comb->text) {//違う要素で同じ内容があった
				ranklist.erase(comb);
				comb = ranklist.begin();
				coma = ranklist.begin();//最初から調査し直し
			}
			else { ++comb; }
			}
		++coma;
		}
	}
	cout << endl;

	cout << "おまたせしました。結果発表～＾＾" << endl;
	for (int cnt = 0;; cnt++) {
		if (ranklist.size() == 0)break;

		cout << "数が" << cnt << "だった奴～～～ !!" << endl;

		rankitr = ranklist.begin();
		while (rankitr!=ranklist.end()) {
			if (rankitr->count == cnt) {
				cout << rankitr->text << endl;
				rankitr = ranklist.erase(rankitr);
			}
			else { ++rankitr; }
		}
		cout << endl;
	}
	cout << "お疲れ様でした～～～" << endl;
	getchar();
    return 0;
}

