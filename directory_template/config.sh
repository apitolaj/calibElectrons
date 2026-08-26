source "$(dirname "${BASH_SOURCE[0]}")/../../home_path.sh"

#if the user's home path was entered correctly in home_path.sh, there is no need to alter any of the paths below.

SOURCE_FAMILY="Source_SOURCE_PLACEHOLDER"

SOURCE="SOURCE_PLACEHOLDER"

SIMU_DATA="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/DATA/brios"

SNCUTS_SCRIPT="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/submit_SNCUTS_array.sh"

MY_DIR="${HOME_PATH}/configuration"

SRC_BASE_SN="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/DATA/brios"

DST_BASE_SN="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/DATA/brios/SNCUTS/"

WORKER_SN="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/run_SNCUTS.sh"

MIMODULE_SCRIPT="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/submit_MiModule_array.sh"

SRC_BASE_MI="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/DATA/brios/SNCUTS/"

DST_BASE_MI="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/DATA/brios/SNCUTS/ROOT/"

WORKER_MI="${HOME_PATH}/SOURCES/${SOURCE_FAMILY}/run_MiModule.sh"

CONF_MI="${HOME_PATH}/configuration/p_MiModule_v00.conf"

