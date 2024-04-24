<?
date_default_timezone_set("Europe/paris");
define ("APP_PATH", realpath(substr(dirname(__FILE__), 0, 0 - strlen("config")))."/");

// define("APP_PATH","/Library/WebServer/Documents/acquimetre/app");

define("WWW_PATH",APP_PATH . "/www");
define ("LIB_PATH", APP_PATH . "/libs");
define ("FONTS_PATH", APP_PATH . "/fonts");
define ("CONTRATS_PATH", APP_PATH . "/contrats");
define ("RESSOURCES_PATH", APP_PATH . "/ressources");

define("APP_URL","");
$TICKETS_VALUES_FILE = APP_PATH . "/config/tickets.conf";

$CDEBUG_FILENAME = APP_PATH. "/logs/debug.log";
$CPROGRAMME_PATH = APP_PATH. "/programmes";
$CSOCLES_PATH = APP_PATH. "/socles";
include(APP_PATH . "/class/classloader.inc");

CTemplate::openModule("default");
CTemplate::openModule("contacts");
CTemplate::openModule("cgventes");
CTemplate::openModule("connexion");
CTemplate::openModule("forum");
CTemplate::openModule("tarifs");

CTemplate::setLoginModule("connexion");

session_name("MEZEVAL");

CDatabase::mySql(null,"mezeval_demo","root","");

putenv('GDFONTPATH=' . APP_PATH . '/fonts');

define("FICHIER_BON_DE_COMMANDE","2008-09.inc");
define("FICHIER_CONDITIONS_DE_VENTES","2009-04.inc");

include("static_tickets.conf");
