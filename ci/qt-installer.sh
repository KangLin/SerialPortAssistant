#!/bin/bash

#http://stackoverflow.com/questions/25105269/silent-install-qt-run-installer-on-ubuntu-server
#http://doc.qt.io/qtinstallerframework/noninteractive.html
#参考：https://github.com/benlau/qtci
#     https://github.com/rabits/dockerfiles
#     https://github.com/mjscosta/qt-silent-installer

set -e #quit on error

function usage() {
    echo "Usage:"
    echo "    qt-installer.sh qt-installer-file output_path"
    echo "    extract-qt-installer --list-packages qt-installer"
    echo "Environment Variable:"
    echo "    QT_USER:     qt user"
    echo "    QT_PASSWORD: user password"
    echo "    QT_SELECT_COMPONENTS: be will installed packages of separated by commas"
    exit -1
}

if [ "$GETOPT_RET_CODE" != "4" ]
then
    echo "Warning: gnu-getopt is not installed. Long parameter like '--list-package' will not be working. Please install gnu-getopt by 'brew install gnu-getopt'"   
else

	OPTS=`getopt -o l --long list-packages --long disable-progress-report -n "extract-qt-installer" -- "$@"`

    eval set -- "$OPTS"

    while true
    do
      case "$1" in
        --list-packages)
          LIST_PACKAGES=1
          shift;;
        --) shift;break;;
        *) shift;;
      esac
    done
fi

export PATH=$PATH:$PWD
export WORKDIR=$PWD
INSTALLER=$1
OUTPUT=$2
SCRIPT="$(mktemp /tmp/tmp.XXXXXXXXX)"
LIST_PACKAGES=0

if [ $LIST_PACKAGES -gt 0 ]
then 

	if [ $# -lt 1 ]
	then
		usage
	fi
	
	OUTPUT="/tmp/Qt"

else

	if [ $# -lt 2 ]
	then
		usage
	fi
    
fi

if [ -n "$QT_CI_PACKAGES" ]; then
    QT_SELECT_COMPONENTS=$QT_CI_PACKAGES
fi

cat <<EOF > $SCRIPT
function abortInstaller()
{
    installer.setDefaultPageVisible(QInstaller.Introduction, false);
    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
    installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
    installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
    installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
    var abortText = "<font color='red' size=3>" + qsTr("Installation failed:") + "</font>";
    var error_list = installer.value("component_errors").split(";;;");
    abortText += "<ul>";
    // ignore the first empty one
    for (var i = 0; i < error_list.length; ++i) {
        if (error_list[i] !== "") {
            log(error_list[i]);
            abortText += "<li>" + error_list[i] + "</li>"
        }
    }
    abortText += "</ul>";
    installer.setValue("FinishedText", abortText);
}
function log() {
    var msg = ["QTCI: "].concat([].slice.call(arguments));
    console.log(msg.join(" "));
}
function printObject(object) {
	var lines = [];
	for (var i in object) {
		lines.push([i, object[i]].join(" "));
	}
	log(lines.join(","));
}
var status = {
	widget: null,
	finishedPageVisible: false,
	installationFinished: false
}
function tryFinish() {
	if (status.finishedPageVisible && status.installationFinished) {
        if (status.widget.LaunchQtCreatorCheckBoxForm) {
            // Disable this checkbox for minimal platform
            status.widget.LaunchQtCreatorCheckBoxForm.launchQtCreatorCheckBox.setChecked(false);
        }
        if (status.widget.RunItCheckBox) {
            // LaunchQtCreatorCheckBoxForm may not work for newer versions.
            status.widget.RunItCheckBox.setChecked(false);
        }
        log("Press Finish Button");
	    gui.clickButton(buttons.FinishButton);
	}
}
function Controller() {
    installer.installationFinished.connect(function() {
		status.installationFinished = true;
        gui.clickButton(buttons.NextButton);
        tryFinish();
    });
    installer.setMessageBoxAutomaticAnswer("OverwriteTargetDirectory", QMessageBox.Yes);
    installer.setMessageBoxAutomaticAnswer("installationErrorWithRetry", QMessageBox.Ignore);
    installer.setMessageBoxAutomaticAnswer("XcodeError", QMessageBox.Ok);
    
    // Allow to cancel installation for arguments --list-packages
    installer.setMessageBoxAutomaticAnswer("cancelInstallation", QMessageBox.Yes);
}
Controller.prototype.WelcomePageCallback = function() {
    log("Welcome Page");
    gui.clickButton(buttons.NextButton);
    var widget = gui.currentPageWidget();
	/* 
	   Online installer 3.0.6 
	   - It must disconnect the completeChanged callback after used, otherwise it will click the 'next' button on another pages
	 */
    var callback = function() {
        gui.clickButton(buttons.NextButton);
        widget.completeChanged.disconnect(callback);
	}
    widget.completeChanged.connect(callback);
}
Controller.prototype.CredentialsPageCallback = function() {
	
	var login = installer.environmentVariable("QT_USER");
	var password = installer.environmentVariable("QT_PASSWORD");
	if (login === "" || password === "") {
		gui.clickButton(buttons.CommitButton);
	}
	
    var widget = gui.currentPageWidget();
	widget.loginWidget.EmailLineEdit.setText(login);
	widget.loginWidget.PasswordLineEdit.setText(password);
    gui.clickButton(buttons.CommitButton);
}
Controller.prototype.ObligationsPageCallback = function() {
    var page = gui.pageWidgetByObjectName("ObligationsPage");
    page.obligationsAgreement.setChecked(true);
    page.completeChanged();
    gui.clickButton(buttons.NextButton);
}
Controller.prototype.StartMenuDirectoryPageCallback = function() {
    console.log("qs:StartMenuDirectoryPageCallback");
    gui.clickButton(buttons.NextButton);
}
Controller.prototype.ComponentSelectionPageCallback = function() {
    log("ComponentSelectionPageCallback");
    function list_packages() {
      var components = installer.components();
      log("Available components: " + components.length);
      var packages = ["Packages: "];
      for (var i = 0 ; i < components.length ;i++) {
          packages.push(components[i].name);
      }
      log(packages.join(" "));
    }
      
    if ($LIST_PACKAGES) {
        list_packages();
        gui.clickButton(buttons.CancelButton);
        return;
    }
    log("Select components");
    function trim(str) {
        return str.replace(/^ +/,"").replace(/ *$/,"");
    }
    var widget = gui.currentPageWidget();
    var packages = trim("$QT_SELECT_COMPONENTS").split(",");
    if (packages.length > 0 && packages[0] !== "") {
        widget.deselectAll();
        var components = installer.components();
        var allfound = true;
        for (var i in packages) {
            var pkg = trim(packages[i]);
            var found = false;
            for (var j in components) {
                if (components[j].name.indexOf(pkg) != -1) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                allfound = false;
                log("ERROR: Package " + pkg + " not found.");
            } else {
                log("Select " + pkg);
                widget.selectComponent(pkg);
            }
        }
        if (!allfound) {
            list_packages();
            // TODO: figure out how to set non-zero exit status.
            gui.clickButton(buttons.CancelButton);
            return;
        }
    } else {
       log("Use default component list");
    }
    gui.clickButton(buttons.NextButton);
}
Controller.prototype.IntroductionPageCallback = function() {
    log("Introduction Page");
    log("Retrieving meta information from remote repository");
    
   	/* 
	   Online installer 3.0.6 
	   - Don't click buttons.NextButton directly. It will skip the component selection.
    */
    
	if (installer.isOfflineOnly()) {
		gui.clickButton(buttons.NextButton);
	}  
}
Controller.prototype.TargetDirectoryPageCallback = function() {
    log("Set target installation page: $OUTPUT");
    var widget = gui.currentPageWidget();
    if (widget != null) {
        widget.TargetDirectoryLineEdit.setText("$OUTPUT");
    }
    
    gui.clickButton(buttons.NextButton);
}
Controller.prototype.LicenseAgreementPageCallback = function() {
    log("Accept license agreement");
    var widget = gui.currentPageWidget();
    if (widget != null) {
        widget.AcceptLicenseRadioButton.setChecked(true);
    }
    gui.clickButton(buttons.NextButton);
}
Controller.prototype.ReadyForInstallationPageCallback = function() {
    log("Ready to install");
    // Bug? If commit button pressed too quickly finished callback might not show the checkbox to disable running qt creator
    // Behaviour started around 5.10. You don't actually have to press this button at all with those versions, even though gui.isButtonEnabled() returns true.
    
    gui.clickButton(buttons.CommitButton, 200);
}
Controller.prototype.PerformInstallationPageCallback = function() {
    log("PerformInstallationPageCallback");
    gui.clickButton(buttons.CommitButton);
}
Controller.prototype.FinishedPageCallback = function() {
    log("FinishedPageCallback");
    var widget = gui.currentPageWidget();
	// Bug? Qt 5.9.5 and Qt 5.9.6 installer show finished page before the installation completed
	// Don't press "finishButton" immediately
    
	status.finishedPageVisible = true;
	status.widget = widget;
	tryFinish();   
}
EOF

chmod u+x $1
#显示log
$1 -v --script $SCRIPT
#不显示log
#$1 --script $SCRIPT
