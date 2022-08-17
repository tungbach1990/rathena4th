/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 100413
 Source Host           : localhost:3306
 Source Schema         : ragnarok

 Target Server Type    : MySQL
 Target Server Version : 100413
 File Encoding         : 65001

 Date: 08/08/2022 23:01:21
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for bachnt_moblooter
-- ----------------------------
DROP TABLE IF EXISTS `bachnt_moblooter`;
CREATE TABLE `bachnt_moblooter`  (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `time` datetime(0) NULL DEFAULT NULL,
  `uniqueid` int(10) NULL DEFAULT NULL,
  `mobid` int(10) NULL DEFAULT NULL,
  `nameid` int(10) NULL DEFAULT NULL,
  `amount` int(10) NULL DEFAULT NULL,
  `refine` int(2) NULL DEFAULT NULL,
  `card0` int(10) NULL DEFAULT NULL,
  `card1` int(10) NULL DEFAULT NULL,
  `card2` int(10) NULL DEFAULT NULL,
  `card3` int(10) NULL DEFAULT NULL,
  `option_id0` int(10) NULL DEFAULT NULL,
  `option_val0` int(10) NULL DEFAULT NULL,
  `option_parm0` int(10) NULL DEFAULT NULL,
  `option_id1` int(10) NULL DEFAULT NULL,
  `option_val1` int(10) NULL DEFAULT NULL,
  `option_parm1` int(10) NULL DEFAULT NULL,
  `option_id2` int(10) NULL DEFAULT NULL,
  `option_val2` int(10) NULL DEFAULT NULL,
  `option_parm2` int(10) NULL DEFAULT NULL,
  `option_id3` int(10) NULL DEFAULT NULL,
  `option_val3` int(10) NULL DEFAULT NULL,
  `option_parm3` int(10) NULL DEFAULT NULL,
  `option_id4` int(10) NULL DEFAULT NULL,
  `option_val4` int(10) NULL DEFAULT NULL,
  `option_parm4` int(10) NULL DEFAULT NULL,
  `enchantgrade` int(2) NULL DEFAULT NULL,
  `claim` int(10) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 121 CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
