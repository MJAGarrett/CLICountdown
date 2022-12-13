#pragma once

class Round
{
public:
  virtual void startRound() = 0;
  virtual int getPlayerScore() = 0;
  virtual void cleanupRound() = 0;
};