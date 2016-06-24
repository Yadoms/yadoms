# <a name="sommaire"></a>Sommaire

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




# <a name="keyword-value-get"></a>Obtenir la valeur d'un équipement

Block permettant d'obtenir la valeur d'un équipement.

Il suffit de choisir l'équipement cible.

![](/this/keyword_get_simple.png)

Le block change automatiquement de type en fonction de l'équipement/donnée choisi.

*Exemple pour un booléen*

![](/this/keyword_get_boolean.png)

*Exemple pour une valeur numérique*

![](/this/keyword_get_value.png)

*[Revenir au sommaire](#sommaire)*

# <a name="keyword-value-set"></a>Affecter une valeur à un équipement

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



# <a name="yadoms_wait_for_event"></a>**Attente d'évènements**

Block permettant d'attendre un ou plusieurs évènements simultanément.

Un évènement peut être de plusieurs types:
 * apparition d'une donnée d'un capteur : par exemmple attente de la prochaine température de mon salon
 * apparition d'une donnée d'un capteur avec une condition : par exemmple attente de la prochaine température de mon salon supérieure à 35°C
 * apparition d'un type de donnée : par exemmple attente de la prochaine donnée de température (tous équipements confondus)
 * apparition d'un type de donnée avec une condition : par exemmple attente de la prochaine donnée de température (tous équipements confondus) supérieure à 35°C
 * attente d'une date/heure spécifique : par exemple : effectuer des actions à 12h42 tous les mercredi
 * action à effectuer toutes les minutes

## Configuration du block

Il s'agit d'utiliser l'engrenage du block pour éditer son comportement.
 
![](/this/wait-for-event-data.gif)



*[Revenir au sommaire](#sommaire)*

# <a name="yadoms_wait_for_event_result"></a>Informations de l'évènement

Block permettant d'obtenir les informations concernant l'évènement en cours.

Cela permet nottament de connaître quel équipement/donnée a généré l'évènement; ainsi que d'obtenir sa valeur.

![](/this/wait_for_event_result.png)

*Exemple*
Voici un script qui créé le texte "Le niveau de batterie de 'Centrale météo Oregon' est faible 4%"

![](/this/wait_for_event_result_exemple.png)

*[Revenir au sommaire](#sommaire)*







# <a name="yadoms_sleep"></a>Temporisation


Block permettant de mettre la règle en attente pendant un certain temps.

*Exemple : attendre 10 secondes*

![](/this/yadoms_sleep.png)

L'unité est configurable et permet d'attendre un certain nombre de
 * secondes
 * minutes
 * heures
 * jours

*[Revenir au sommaire](#sommaire)*

# <a name="yadoms_log"></a>Journalisation(log)

Block qui effectue la journalistion d'une information.
Le texte est enregistré dans le fichier log associé au script en cours.

![](/this/yadoms_log.png)

*[Revenir au sommaire](#sommaire)*

# <a name="yadoms_notification_simple"></a>Notifier(log)

Block qui permet de notifier une personne.

Les moyens de notification sont fourni par le plugins.
Cela peut être un envoi de mail, un envoi de SMS,...

![](/this/yadoms_notification_simple.png)

*[Revenir au sommaire](#sommaire)*


