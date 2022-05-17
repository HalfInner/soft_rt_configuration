# <p align="center"> Propozycja implementacji systemu Soft Real Time Linux bez udziału mikrojąda RTLinux dla aplikacji z rygorem wykonywania zadania poniżej 1ms </p>
## <p align="center">Projekt Zespołowy </br>Politechnika Warszawska </br>OKNO </p>

## Zespół
Skład zespołu:
* Kajetan Brzuszczak 301023 (KB)
* Kajetan Witkowski 299274 (KW)
* Ryszard Biernacki 261410 (RB)

Kajetan Brzuszczak będzie pełnił rolę kierownika projektu.


## Założenia projektu
Problemy zadań czasu rzeczywistego rozwiązywane są różnymi sposobami. Popularnym wysokopoziomowym rozwiązaniem jest skompilowanie i wgranie mikrojądra RTLinux na licencji OpenSource. Niestety ingerencja w jądro systemu nie zawsze jest możliwa, dlatego celem tego projektu jest zbadanie możliwości wbudowanego kernela dla jednej ze znanych dystrybucji i manipulowanie zadaniami lub procesami z poziomu konsoli lub aplikacji korzystającej z glibc, tak, aby wykonywały się w przewidywalnym czasie. Wyzwaniem będzie izolowanie aplikacji pomiędzy rdzeniami, gdzie domyślnie ta opcja jest ograniczona bez instalacji mikrojądra.

Zakładamy, że celem jest Soft Real Time, gdzie przekroczenie czasu wykonania nie skutkuje krytycznym błędem, ale obniża znacznie jakość usługi. Punktem odniesienia będzie wykonywanie zadań w ciągu 1ms.
Podsumowanie będzie zawierało analizę sukcesu projektu.


## Współpraca
W celu usprawnienia współpracy wybraliśmy parę narzędzi
* Komunikator Messenger firmy Facebook, gdzie mogliśmy szybko wymieniać wiadomości
* Microsoft Teams w celu prowadzenia wideokonferencji i polepszenia kondycji zespołu w pracy zdalnej 
* Trello, czyli tablicę Kanban w celu wizualnego podziału obowiązków. Każda osoba z zespołu mogła wziąć jeden element backlogu aby nad nim pracować. 
* Pliki trzymaliśmy w chmurze na dysku OneDrive, gdzie każdy mógł wrzucać wyniki swojej pracy

Przykładowy Screen z rozpisaniem zadań pod projekt
[!Przykład](res/trello.png)

## Systemy Real Time

### Analiza porównawcza systemu Hard i Soft
### Analiza porównawcza systemu użytkowego i Soft

## Weryfikacja Systemu Soft Real Time

Weryfikacja:
* Izolacja CPU  
  ![isolated_cpu](logs/isolated_cpu.png "isolated_cpu")
* Izolacja CPU z dodatkowym obciążeniem systemu  
  ![isolated_cpu_stress](logs/isolated_cpu_stress.png "isolated_cpu_stress")
* Izolacja CPU z dodatkowym obciążeniem systemu, bez przypisania zadań do odizolowanego CPU  
  ![isolated_cpu_stress_no_affinity](logs/isolated_cpu_stress_no_affinity.png "isolated_cpu_stress_no_affinity")
### Aplikacja

### Skrypty

# Bibiliografia
* Sched - Manual Linux  
https://man7.org/linux/man-pages/man7/sched.7.html 
* Sched Affinity – Manual Linux  
https://linux.die.net/man/2/sched_setaffinity 
* Cron Manual  
https://man7.org/linux/man-pages/man8/cron.8.html
* RT Linux Manual  
https://wiki.t-firefly.com/en/Firefly-Linux-Guide/manual_rtlinux.html

