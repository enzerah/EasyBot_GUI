#include "ScriptEditorDialog.h"
#include <QGroupBox>
#include "Highlighter.h"
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>

ScriptEditorDialog::ScriptEditorDialog(const QString &name, const QString &script, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Script Editor");
    resize(1000, 600);

    auto mainLayout = new QHBoxLayout(this);

    auto layout = new QVBoxLayout();

    auto nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Script Name:", this));
    m_nameEdit = new QLineEdit(name, this);
    nameLayout->addWidget(m_nameEdit);
    layout->addLayout(nameLayout);

    m_codeEditor = new CodeEditor(this);
    m_codeEditor->setPlainText(script);
    new Highlighter(m_codeEditor->document());

    layout->addWidget(m_codeEditor);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);

    // Sripts Helper
    auto scriptsGridLayout = new QGridLayout();
    
    auto setupGroup = [&](QGroupBox* group, QListWidget*& list) {
        auto gLayout = new QVBoxLayout(group);
        list = new QListWidget(group);
        gLayout->addWidget(list);
    };

    auto g_game = new QGroupBox("Game", this);
    setupGroup(g_game, m_gameList);

    auto g_localPlayer = new QGroupBox("LocalPlayer", this);
    setupGroup(g_localPlayer, m_localPlayerList);

    auto g_container = new QGroupBox("Container", this);
    setupGroup(g_container, m_containerList);

    auto g_creature = new QGroupBox("Creature", this);
    setupGroup(g_creature, m_creatureList);

    auto g_item = new QGroupBox("Item", this);
    setupGroup(g_item, m_itemList);

    auto g_map = new QGroupBox("Map", this);
    setupGroup(g_map, m_mapList);

    auto g_thing = new QGroupBox("Thing", this);
    setupGroup(g_thing, m_thingList);

    auto g_tile = new QGroupBox("Tile", this);
    setupGroup(g_tile, m_tileList);

    scriptsGridLayout->addWidget(g_game ,0, 0);
    scriptsGridLayout->addWidget(g_localPlayer, 0, 1);
    scriptsGridLayout->addWidget(g_creature, 1, 0);
    scriptsGridLayout->addWidget(g_container, 1, 1);
    scriptsGridLayout->addWidget(g_item, 2, 0);
    scriptsGridLayout->addWidget(g_map, 2, 1);
    scriptsGridLayout->addWidget(g_thing, 3, 0);
    scriptsGridLayout->addWidget(g_tile, 3, 1);


    mainLayout->addLayout(layout, 1);
    mainLayout->addLayout(scriptsGridLayout, 2);

    populateLists();

    auto connectList = [&](QListWidget* list) {
        connect(list, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
            auto text = item->text();
            int index = text.indexOf(" ->");
            if (index != -1) {
                text = text.left(index);
            }
            text += "\n";
            m_codeEditor->insertPlainText(text);
        });
    };

    connectList(m_gameList);
    connectList(m_localPlayerList);
    connectList(m_containerList);
    connectList(m_creatureList);
    connectList(m_itemList);
    connectList(m_mapList);
    connectList(m_thingList);
    connectList(m_tileList);
}

void ScriptEditorDialog::populateLists() {
    m_gameList->addItems({
        "getLocalPlayer() -> LocalPlayer",  "countItems({id1, id2}) -> {numbers}", "walk(Otc::Direction)", "turn(Otc::Direction)", "stop()",
        "move(Thing, Position, count)", "moveToParentContainer(Thing, count)", "use(Thing)", "safeLogout()",
        "useWith(Item, Thing)", "useInventoryItem(itemId)", "useInventoryItemWith(itemId, Thing)",
        "findItemInContainers(itemId, subType, tier) -> Item", "open(Item, previousContainer) -> number",
        "openParent(Container)", "close(Container)", "refreshContainer(Container)",
        "attack(Creature, cancel)", "cancelAttack()", "follow(Creature)", "cancelAttackAndFollow()",
        "talk(message)", "talkChannel(Otc::MessageMode, channelId, message)", "talkPrivate(Otc::MessageMode, receiver, message)",
        "openPrivateChannel(receiver)", "setChaseMode(Otc::ChaseModes)", "setFightMode(Otc::FightModes)",
        "buyItem(Item, amount, ignoreCapacity, buyWithBackpack)", "sellItem(Item, amount, ignoreEquipped)",
        "equipItem(Item)", "equipItemId(itemId, tier)", "mount(state)", "changeMapAwareRange(xrange, yrange)",
        "canPerformGameAction() -> bool", "isOnline() -> bool", "isAttacking() -> bool", "isFollowing() -> bool", "getContainer(Id) -> Container",
        "getContainers() -> ContainerList", "getAttackingCreature() -> Creature", "getFollowingCreature() -> Creature",
        "getClientVersion() -> number", "getCharacterName() -> string"
    });
    
    m_localPlayerList->addItems({
        "getStates(LocalPlayer) -> number", "getHealth(LocalPlayer) -> number", "getMaxHealth(LocalPlayer) -> number", 
        "getFreeCapacity(LocalPlayer) -> number", "getLevel(LocalPlayer) -> number", "getMana(LocalPlayer) -> number", 
        "getMaxMana(LocalPlayer) -> number", "getManaShield(LocalPlayer) -> number", "getSoul(LocalPlayer) -> number", 
        "getStamina(LocalPlayer) -> number", "getInventoryItem(LocalPlayer, Otc::InventorySlot) -> Item", 
        "getInventoryCount(LocalPlayer, itemId, tier) -> number", 
        "hasSight(LocalPlayer, Position) -> bool", "isAutoWalking(LocalPlayer) -> bool", "stopAutoWalk(LocalPlayer)", 
        "autoWalk(LocalPlayer, Position, retry) -> bool", "setLightHack(LocalPlayer, lightLevel)"
    });
    
    m_containerList->addItems({
        "getItem(Container, slot) -> Item", "getItems(Container) -> ItemList", "getItemsCount(Container) -> number", 
        "getSlotPosition(Container, slot) -> Position", "getContainerName(Container) -> string", "getContainerId(Container) -> number", 
        "getContainerItem(Container) -> Item", "hasParent(Container) -> bool", "getCapacity(Container) -> number", "getFirstIndex(Container) -> number"
    });
    
    m_creatureList->addItems({
        "getCreatureName(Creature) -> string", "getHealthPercent(Creature) -> number", "getDirection(Creature) -> number", 
        "isDead(Creature) -> bool", "canBeSeen(Creature) -> bool", "isCovered(Creature) -> bool", "canShoot(Creature, distance) -> bool"
    });
    
    m_itemList->addItems({
        "getCount(Item) -> number", "getSubType(Item) -> number", "getItemId(Item) -> number", "getTooltip(Item) -> string", 
        "getDurationTime(Item) -> number", "getItemName(Item) -> string", "getDescription(Item) -> string", "getTier(Item) -> number", "getText(Item) -> string"
    });
    
    m_mapList->addItems({
        "getTile(Position) -> Tile", "getSpectators(Position, multiFloor) -> CreatureList", 
        "isWalkable(Position, ignoreCreatures) -> bool", "isSightClear(fromPosition, toPosition) -> bool"
    });
    
    m_thingList->addItems({
        "getId(Thing) -> number", "getPosition(Thing) -> Position", "getParentContainer(Thing) -> Container", "isItem(Thing) -> bool", 
        "isMonster(Thing) -> bool", "isNpc(Thing) -> bool", "isCreature(Thing) -> bool", "isPlayer(Thing) -> bool", 
        "isLocalPlayer(Thing) -> bool", "isContainer(Thing) -> bool", "isUsable(Thing) -> bool", "isLyingCorpse(Thing) -> bool"
    });
    
    m_tileList->addItems({
        "getTopThing(Tile) -> Thing", "getTopUseThing(Tile) -> Thing", "getTileItems(Tile) -> ItemList"
    });
}

QString ScriptEditorDialog::getScript() const {
    return m_codeEditor->toPlainText();
}

QString ScriptEditorDialog::getName() const {
    return m_nameEdit->text();
}
