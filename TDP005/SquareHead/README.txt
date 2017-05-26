Spelet kan laddas ner genom att köra följande kommando:
       git clone git@gitlab.ida.liu.se:maxbr431/not-smashtv.git

Dessa libraries behövs för att kompilera spelet:
      SDL2
      SDL2_image
      SDL2_ttf

Följade "make"-kommandon finns tillgängliga:

     make - kompilerar programmet.

     make run - kompilerar (om inte redan kompilerat) och kör programmet.

     make clean - tar bort alla skapade filer förutom highscore-filen.

     make zap - tar bort alla skapade filer.

För att köra spelet utan "make run" så kan du (efter kompilering) skriva in:

    ./SquareHead

Kontrollschemat i spelet är följande:
     
     Spelet:
     w - rör spelaren uppåt
     s - rör spelaren neråt
     a - rör spelaren åt vänster
     d - rör spelaren åt höger
     space - spelaren skjuter
     
     Menyer:
     Enter - i menyerna är det kortkommandot för den översta knappen.
     Escape - i huvudmenyn avslutas spelet om Escape trycks ner.
     Mus - används för att klicka på knappar.
