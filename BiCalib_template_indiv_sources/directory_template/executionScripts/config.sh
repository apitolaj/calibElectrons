source "../../home_path.sh"

#if the user's home path was entered correctly in home_path.sh, there is no need to alter any of the paths below nor any of the paths in the config files.

SOURCE_FAMILY="Source_SOURCE_PLACEHOLDER"

SOURCE="SOURCE_PLACEHOLDER"

SIMU_DATA="${HOME_PATH}/${SOURCE_FAMILY}/DATA/brios"

SNCUTS_SCRIPT="${HOME_PATH}/${SOURCE_FAMILY}/executionScripts/submit_SNCUTS_on_c2e_array.sh"

MY_DIR="${HOME_PATH}/configuration"

SRC_BASE_SN="${HOME_PATH}/${SOURCE_FAMILY}/DATA/brios"

DST_BASE_SN="${HOME_PATH}/${SOURCE_FAMILY}/DATA/brios/SNCUTS/"

WORKER_SN="${HOME_PATH}/${SOURCE_FAMILY}/executionScripts/run_SNCUTS_on_c2e_array.sh"

MIMODULE_SCRIPT="${HOME_PATH}/${SOURCE_FAMILY}/executionScripts/submit_MiModule.sh"

SRC_BASE_MI="${HOME_PATH}/${SOURCE_FAMILY}/DATA/brios/SNCUTS/"

DST_BASE_MI="${HOME_PATH}/${SOURCE_FAMILY}/DATA/brios/SNCUTS/ROOT/"

WORKER_MI="${HOME_PATH}/${SOURCE_FAMILY}/executionScripts/run_MiModule.sh"

CONF_MI="${HOME_PATH}/configuration/p_MiModule_v00.conf"

