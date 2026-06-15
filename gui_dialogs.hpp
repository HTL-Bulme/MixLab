#ifndef GUI_DIALOGS_HPP
#define GUI_DIALOGS_HPP

#include "language.hpp"
#include "ui_state.hpp"

namespace mixlab {

void showAboutDialog(Language language);
void showHelpDialog(Language language);
void showValenceDialog(Language language);
void showElementTableDialog(Language language);
void showAllReactionsDialog(Language language);
void showDangerousReactionsDialog(Language language);
void showSafeReactionsDialog(Language language);
void showSettingsDialog(Language language);
void showSaveReactionDialog(UiState& state, Language language);
void showSavedReactionsDialog(const UiState& state, Language language);
void showExportDialog(const ReactionResult& currentResult, Language language);

} // namespace mixlab

#endif // GUI_DIALOGS_HPP
