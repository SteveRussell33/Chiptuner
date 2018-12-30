#include "ChipTuner.hpp"

struct BlankBaseWidget : ModuleWidget {
	static constexpr int LISTSIZE = 3;
	int selected = 0;
	std::string fileName[LISTSIZE];
	BitMap *bmp;
	std::string FileName(std::string tpl, int templateSize) {
		char workingSpace[100];
		snprintf(workingSpace, 100, tpl.c_str(), templateSize);
		return assetPlugin(plugin, workingSpace);
	}

	BlankBaseWidget(Module *module) : ModuleWidget(module) { }
	void appendContextMenu(Menu *menu) override;
	void loadBitmap() {
		bmp = Widget::create<BitMap>(Vec(0,0));
		bmp->box.size.x = box.size.x;
		bmp->box.size.y = box.size.y;
		bmp->path = fileName[selected];
		addChild(bmp);
	}
	void setBitmap(int sel) {
		if (selected == sel)
			return;
		selected = clamp(sel, 0, LISTSIZE - 1);
		removeChild(bmp);
		delete bmp;
		loadBitmap();
	}
	json_t *toJson() override {
		json_t *rootJ = ModuleWidget::toJson();
		json_object_set_new(rootJ, "style", json_real(selected));
		return rootJ;
	}
	void fromJson(json_t *rootJ) override {
		ModuleWidget::fromJson(rootJ);
		int sel = selected;
		json_t *styleJ = json_object_get(rootJ, "style");
		if (styleJ)
			sel = json_number_value(styleJ);
		setBitmap(sel);
	}	
	
};

struct BitmapMenuItem : MenuItem {
	BlankBaseWidget *w;
	int value;
	void onAction(EventAction &e) override {
		w->setBitmap(value);
	}
};

void BlankBaseWidget::appendContextMenu(Menu *menu) {
	menu->addChild(MenuEntry::create());
	BitmapMenuItem *m = MenuItem::create<BitmapMenuItem>("Base");
	m->w = this;
	m->value = 0;
	m->rightText = CHECKMARK(selected==m->value);
	menu->addChild(m);
	m = MenuItem::create<BitmapMenuItem>("Alternative");
	m->w = this;
	m->value = 1;
	m->rightText = CHECKMARK(selected==m->value);
	menu->addChild(m);
	m = MenuItem::create<BitmapMenuItem>("Extreme");
	m->w = this;
	m->value = 2;
	m->rightText = CHECKMARK(selected==m->value);
	menu->addChild(m);	
}

template<int x>
struct BlankWidget : BlankBaseWidget {
	BlankWidget(Module *module) : BlankBaseWidget(module) {
		fileName[0] = FileName("res/8Bit_%dHP.png", x);
		fileName[1] = FileName("res/8Bit_Alt_%dHP.png", x);
		fileName[2] = FileName("res/8Bit_Ext_%dHP.png", x);		
		box.size = Vec(RACK_GRID_WIDTH * x, RACK_GRID_HEIGHT);
		loadBitmap();
	}
};

#define MODEL(x) Model *modelBlank_##x##HP = Model::create<Module, BlankWidget<x>>("ChipTuner", "Blank " #x "HP", #x "HP Blanking Plate", BLANK_TAG);
MODEL(1)
MODEL(2)
MODEL(3)
MODEL(4)
MODEL(6)
MODEL(10)
MODEL(12)
MODEL(16)
MODEL(20)
MODEL(26)
MODEL(32)
