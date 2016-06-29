<div class="bg-primary">

# <a name="sommaire"></a>Sommaire

</div>


## Gestion des équipements

 * [Obtenir la valeur d'un équipement](#obtenir-la-valeur-dun-equipement)
 * [Affecter une valeur à un équipement](#affecter-une-valeur-a-un-equipement)

## Évènements

 * [**Attente d'évènements**](#yadoms_wait_for_event)
 * [Informations de l'évènement](#yadoms_wait_for_event_result)

## Actions

 * [Temporisation](#yadoms_sleep)
 * [Informations du système](#yadoms_get_info)
 * [Journalisation(log)](#yadoms_log)
 * [Notifier](#yadoms_notification_simple)

## Gestion des dates

 * [Date et heure](#yadoms_date_datetime)
 * [Date](#yadoms_date_date)
 * [Heure](#yadoms_date_time)
 * [Jour de la semaine ](#yadoms_date_weekDay)
 * [Chaque jour](#yadoms_date_everyDay)
 * [Aujourd'hui ](#yadoms_date_today)
 * [Maintenant ](#yadoms_date_now)
 * [Lever du soleil ](#yadoms_date_sunrise)
 * [Coucher du soleil ](#yadoms_date_sunset)
 * [Durée ](#yadoms_date_timespan)
 * [Arithmétique de dates/heures ](#yadoms_date_arithmetic)
 * [Extraction d'information ](#yadoms_date_extract)


<div class="bg-primary">

# <a name="keyword-value-get"></a>Obtenir la valeur d'un équipement

</div>

Block permettant d'obtenir la valeur d'un équipement.

Il suffit de choisir l'équipement cible.

![](/this/keyword_get_simple.png)

Le block change automatiquement de type en fonction de l'équipement/donnée choisi.

*Exemple pour un booléen*

![](/this/keyword_get_boolean.png)

*Exemple pour une valeur numérique*

![](/this/keyword_get_value.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="keyword-value-set"></a>Affecter une valeur à un équipement

</div>

Block permettant d'affecter une nouvelle valeur à un équipement.

Il suffit de choisir l'équipement cible.

![](/this/keyword_set_simple.png)

Ensuite le block se met lui même à jour pour proposer la saisie de la valeur.


*Exemple pour un booléen*

![](/this/keyword_set_boolean.png)

*Exemple pour une valeur numérique*

![](/this/keyword_set_value.png)

*Cas particulier des évènements*

Il est possible que les équipements soient simplement commandables par une action.
Dans ce cas le block change lui même d'intitulé

![](/this/keyword_set_action.png)


*[Revenir au sommaire](#sommaire)*


<div class="bg-primary">

# <a name="yadoms_wait_for_event"></a>**Attente d'évènements**

</div>

Block permettant d'attendre un ou plusieurs évènements simultanément.

Un évènement peut être de plusieurs types:
 * apparition d'une donnée d'un capteur : par exemmple attente de la prochaine température de mon salon
 * apparition d'une donnée d'un capteur avec une condition : par exemmple attente de la prochaine température de mon salon supérieure à 35°C
 * apparition d'un type de donnée : par exemmple attente de la prochaine donnée de température (tous équipements confondus)
 * apparition d'un type de donnée avec une condition : par exemmple attente de la prochaine donnée de température (tous équipements confondus) supérieure à 35°C
 * attente d'une date/heure spécifique : par exemple : effectuer des actions à 12h42 tous les mercredi
 * action à effectuer toutes les minutes

## Fonctionnement du block

Lorsqu'un des évènements configuré est émis, alors le block applique les actions associées puis se termine.

Dans la plupart des cas il est fortement recommandé de poser le block dans une boucle infinie.


## Configuration du block

Il s'agit d'utiliser l'engrenage du block pour éditer son comportement.
Depuis l'interface proposée, on peut choisir quel type d'évènement on attend.

![](/this/wait-for-event-data.gif)

Le block peut se moduler pour attendre plusieurs évènements simultanément.

*Exemple : configuration du block*
![](/this/wait_for_event_data_exemple_multiple.png)

*Résultat après configuration*
![](/this/wait_for_event_data_exemple_multiple_with_selection.png)


Il est possible également d'obtenir des informations à propos de l'évènement en cours de traitement.

Pour cela on utilise le block ["Informations de l'évènement"](#yadoms_wait_for_event_result):

Cela permet d'obtenir 
 * le nom de l'équipement qui a émis la donnée (pratique lorsqu'on attend un type d'évènement)
 * la valeur exacte de la donnée qui a généré l'évènement

*Exemple complet*

![](/this/wait_for_event_data_exemple_complet.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_wait_for_event_result"></a>Informations de l'évènement

</div>

Block permettant d'obtenir les informations concernant l'évènement en cours.

Cela permet nottament de connaître quel équipement/donnée a généré l'évènement; ainsi que d'obtenir sa valeur.

![](/this/wait_for_event_result.png)

*Exemple*
Voici un script qui créé le texte "Le niveau de batterie de 'Centrale météo Oregon' est faible 4%"

![](/this/wait_for_event_result_exemple.png)

*[Revenir au sommaire](#sommaire)*



<div class="bg-primary">

# <a name="yadoms_sleep"></a>Temporisation

</div>

Block permettant de mettre la règle en attente pendant un certain temps.

*Exemple : attendre 10 secondes*

![](/this/yadoms_sleep.png)

L'unité est configurable et permet d'attendre un certain nombre de
 * secondes
 * minutes
 * heures
 * jours

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_log"></a>Journalisation(log)

</div>

Block qui effectue la journalistion d'une information.
Le texte est enregistré dans le fichier log associé au script en cours.

![](/this/yadoms_log.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_notification_simple"></a>Notifier

</div>

Block qui permet de notifier une personne.

Les moyens de notification sont fournis par les plugins.
Cela peut être un envoi de mail, un envoi de SMS,...

![](/this/yadoms_notification_simple.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_datetime"></a>Date et heure

</div>

Block qui permet de spécifier une date et une heure.

![](/this/yadoms_date_datetime.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_date"></a>Date

</div>

Block qui permet de spécifier une date seule

![](/this/yadoms_date_date.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_time"></a>Heure

</div>

Block qui permet de spécifier une heure seule

![](/this/yadoms_date_time.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_weekDay"></a>Jour de la semaine 

</div>

Block qui permet de spécifier un jour particulier de la semaine

![](/this/yadoms_date_weekDay.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_everyDay"></a>Chaque jour

</div>

Block qui permet de spécifier n'importe quel jour de la semaine (pour utilisation avec le block ["Attente d'évènements"](#yadoms_wait_for_event))

![](/this/yadoms_date_everyDay.png)

*[Revenir au sommaire](#sommaire)*


<div class="bg-primary">

# <a name="yadoms_date_today"></a>Aujourd'hui 

</div>

Block qui permet d'obtenir le jour actuel

![](/this/yadoms_date_today.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_now"></a>Maintenant 

</div>

Block qui permet d'obtenir l'heure actuelle

![](/this/yadoms_date_now.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_sunrise"></a>Lever du soleil 

</div>

Block qui permet d'obtenir l'heure de lever du soleil pour aujourd'hui

![](/this/yadoms_date_sunrise.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_sunset"></a>Coucher du soleil 

</div>

Block qui permet d'obtenir l'heure de coucher du soleil pour aujourd'hui

![](/this/yadoms_date_sunset.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_timespan"></a>Durée 

</div>

Block qui permet de définir ne durée.

![](/this/yadoms_date_timespan.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_arithmetic"></a>Arithmétique de dates/heures 

</div>

Block qui permet de réaliser des opérations arithmétiques sur les dates.

![](/this/yadoms_date_arithmetic.png)

*[Revenir au sommaire](#sommaire)*

<div class="bg-primary">

# <a name="yadoms_date_extract"></a>Extraction d'information 

</div>

Block qui permet d'extraire une partie d'une date/heure.

![](/this/yadoms_date_extract.png)

Il est possible d'extraire:
 * la date sans l'heure
 * l'heure complète (foramt HH:MM)
 * l'année
 * le mois
 * le jour
 * l'heure (format HH)
 * les minutes
 * le jour de la semaine

*[Revenir au sommaire](#sommaire)*

