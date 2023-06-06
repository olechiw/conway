#include "ConwayWorker.h"

ConwayWorker::ConwayWorker(ConwayGame* game): _game(game)
{
}

void ConwayWorker::run()
{
	while (true) {
		_game->step();
		emit gameUpdated(_game->getState());
	}
}
