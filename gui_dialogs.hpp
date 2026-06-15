#ifndef GUI_DIALOGS_HPP
#define GUI_DIALOGS_HPP

#include "ui_state.hpp"

namespace mixlab {

void showAboutDialog();
void showHelpDialog();
void showValenceDialog();
void showElementTableDialog();
void showAllReactionsDialog();
void showDangerousReactionsDialog();
void showSafeReactionsDialog();
void showSettingsDialog();
void showSaveReactionDialog(UiState& state);
void showSavedReactionsDialog(const UiState& state);
void showExportDialog(const ReactionResult& currentResult);

} // namespace mixlab

#endif // GUI_DIALOGS_HPP
