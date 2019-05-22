#include "PursuitScene_1.h"

#include <set>

#include "Assets.h"

PursuitScene_1::PursuitScene_1(const sf::Vector2u& worldBounds, ResourceManager& resourceManager) :
	type(Type::DISABLED),
	resourceManager(resourceManager),
	worldBounds(worldBounds),
	actorVelocity(0.f),
	actorSpacing(0.f),
	colliger(resourceManager),
	ghosts(),
	random()
{
}

// This method is fat and does a lot and i'm not proud of it :((((
void PursuitScene_1::Init(
	Type type, 
	Direction entranceDirection, 
	float scaleY, 
	int ghostCount
)
{
	// Fix ghosts.size() to ghostCount
	while (ghosts.size() < ghostCount)
	{
		ghosts.emplace_back(resourceManager);
	}
	while (ghosts.size() > ghostCount)
	{
		ghosts.pop_back();
	}

	// Set ghost styles
	std::list<GhostStyle> ghostStyles = GenerateGhostStyles(type, ghostCount);
	auto ghostsIter = ghosts.begin();
	auto stylesIter = ghostStyles.begin();
	for (; stylesIter != ghostStyles.end(); ghostsIter++, stylesIter++)
	{
		ghostsIter->SetStyle(*stylesIter);
	}

	// Init actors
	float posY = scaleY * worldBounds.y;
	switch (type)
	{
	case COLLIGER_CHASED: // colliger is the lead
	{
		colliger.SetDirection(entranceDirection);
		colliger.SetVelocity(actorVelocity);

		float colligerWidth = colliger.GetBounds().width;
		switch (entranceDirection)
		{
		case LEFT:
		{
			float posX = worldBounds.x + colligerWidth / 2.f + actorSpacing;
			colliger.SetCenterPosition({ posX, posY });
			PursuitActor_1* tail = &colliger;
			for (GhostActor_1& ghost : ghosts)
			{
				ghost.SetDirection(entranceDirection); // Sets correct bounds
				ghost.SetVelocity(actorVelocity);

				float ghostWidth = ghost.GetBounds().width;
				ghost.SetCenterPosition({ tail->GetTail() + actorSpacing + ghostWidth / 2.f, posY });

				tail = &ghost;
			}

			break;
		}
		case RIGHT:
		{
			float posX = -1 * (colligerWidth / 2.f + actorSpacing);
			colliger.SetCenterPosition({ posX, posY });
			PursuitActor_1* tail = &colliger;
			for (GhostActor_1& ghost : ghosts)
			{
				ghost.SetDirection(entranceDirection); // Sets correct bounds
				ghost.SetVelocity(actorVelocity);

				float ghostWidth = ghost.GetBounds().width;
				ghost.SetCenterPosition({ tail->GetTail() - actorSpacing - ghostWidth / 2.f, posY });

				tail = &ghost;
			}

			break;
		}
		default: {} // TODO: warn
		}

		break;
	}
	case COLLIGER_CHASING: // The ghosts are the lead
	{
		colliger.SetDirection(entranceDirection);
		colliger.SetVelocity(actorVelocity);

		for (GhostActor_1& ghost : ghosts)
		{
			ghost.SetDirection(entranceDirection); // Sets correct bounds
			ghost.SetVelocity(actorVelocity);
		}
		
		float ghostWidth = ghosts.begin()->GetBounds().width; // Ghost width should be > 0
		switch (entranceDirection)
		{
		case LEFT:
		{
			float posX = worldBounds.x + ghostWidth / 2.f + actorSpacing;
			for (auto ghostsIter = ghosts.begin(); ghostsIter != ghosts.end(); ghostsIter++)
			{
				if (ghostsIter == ghosts.begin())
				{
					ghostsIter->SetCenterPosition({ posX, posY });
				}
				else
				{
					auto prevIter = std::prev(ghostsIter);
					ghostsIter->SetCenterPosition({ prevIter->GetTail() + actorSpacing + ghostWidth / 2.f, posY });
				}
			}

			float colligerWidth = colliger.GetBounds().width;
			colliger.SetCenterPosition({ ghosts.rbegin()->GetTail() + actorSpacing + colligerWidth / 2.f, posY });

			break;
		}
		case RIGHT:
		{
			float posX = -1 * (ghostWidth / 2.f + actorSpacing);
			for (auto ghostsIter = ghosts.begin(); ghostsIter != ghosts.end(); ghostsIter++)
			{
				if (ghostsIter == ghosts.begin())
				{
					ghostsIter->SetCenterPosition({ posX, posY });
				}
				else
				{
					auto prevIter = std::prev(ghostsIter);
					ghostsIter->SetCenterPosition({ prevIter->GetTail() - actorSpacing - ghostWidth / 2.f, posY });
				}
			}

			float colligerWidth = colliger.GetBounds().width;
			colliger.SetCenterPosition({ ghosts.rbegin()->GetTail() - actorSpacing - colligerWidth / 2.f, posY });

			break;
		}
		default: {} // TODO: warn
		}

		break;
	}
	default: {} // I HATE DOING THIS !!!!
	}

	this->type = type;
	state = PlayState::PRE_PLAY;
	sceneTicker.Reset();
}

void PursuitScene_1::SetActorVelocity(float actorVelocity)
{
	this->actorVelocity = actorVelocity;
}

void PursuitScene_1::SetActorSpacing(float actorSpacing)
{
	this->actorSpacing = actorSpacing;
}

PursuitScene_1::Type PursuitScene_1::GetType() const
{
	return type;
}

Direction PursuitScene_1::GetEntranceDirection() const
{
	return colliger.GetDirection(); // Any actor would suffice
}

void PursuitScene_1::Update(float deltaTime)
{
	UpdateTicker(deltaTime);

	if (state == PlayState::PLAYING)
	{
		colliger.Update(deltaTime);
		for (GhostActor_1& ghost : ghosts)
		{
			ghost.Update(deltaTime);
		}

		PursuitActor_1* tail = nullptr;
		switch (type)
		{
		case COLLIGER_CHASED:
			tail = &*ghosts.rbegin(); // LOL

			break;
		case COLLIGER_CHASING:
			tail = &colliger;

			break;
		default: {}
		}

		if (tail) // I'm a good cpp citizen 
		{
			switch (tail->GetDirection())
			{
			case LEFT:
				if (tail->GetTail() <= -actorSpacing)
				{
					state = PlayState::FINISHED;
				}

				break;
			case RIGHT:
				if (tail->GetTail() >= worldBounds.x + actorSpacing)
				{
					state = PlayState::FINISHED;
				}

				break;
			default: {}
			}
		}
	}
}

void PursuitScene_1::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (state == PlayState::PLAYING) // Only draw when PLAYING
	{
		target.draw(colliger, states);
		for (const GhostActor_1& ghost : ghosts)
		{
			target.draw(ghost, states);
		}
	}
}

std::list<GhostStyle> PursuitScene_1::GenerateGhostStyles(Type type, int ghostCount) const
{
	// THIS IS ABSOLUTELY FUCKED UP !!!!!!!!!

	std::vector<GhostStyle> availStyles;
	std::set<GhostStyle> takenStyles;
	switch (type)
	{
	case COLLIGER_CHASED:
	{
		availStyles =
		{
			GhostStyle::BLUE,
			GhostStyle::RED,
			GhostStyle::PINK,
			GhostStyle::GREEN,
			GhostStyle::YELLO,
			GhostStyle::CYAN
		};

		break;
	}
	case COLLIGER_CHASING:
	{
		availStyles =
		{
			GhostStyle::FRIGHTENED_LOOK,
			GhostStyle::NAKED
		};

		break;
	}
	default: {} // I fucking hate doing this
	}

	// Generate random styles
	std::list<GhostStyle> styles;
	while (styles.size() < ghostCount)
	{
		GhostStyle style;
		// Make sure styles aren't repeated 
		// till all styles are taken
		do
		{
			style = availStyles[random.Next(0, static_cast<int>(availStyles.size() - 1))];
			//::cout << "Style: " << style << std::endl;
			//std::cout << "Lol??? : " << (takenStyles & style) << std::endl;
		} while (styles.size() < availStyles.size() && takenStyles.find(style) != takenStyles.end());

		styles.push_back(style);
		takenStyles.insert(style);
	}

	return styles;
}
