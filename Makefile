include $(TOPDIR)/rules.mk

PKG_NAME:=open-vpn-management
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/open-vpn-management
	CATEGORY:=Base system
	TITLE:=open-vpn-management
	DEPENDS:=+ubox +libubox +libubus +libblobmsg-json
endef

define Package/open-vpn-management/description
	This is an Open VPN management program
endef

define Package/open-vpn-management/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d 
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/open-vpn-management $(1)/usr/bin
	$(INSTALL_BIN) ./files/open-vpn-management.init $(1)/etc/init.d/open-vpn-management
endef

$(eval $(call BuildPackage,open-vpn-management))