#include "UseSpell_Thread.h"


void UseSpell_Thread::run() {
    if (m_spells.empty()) return;
    while (!isInterruptionRequested()) {

    }
}
