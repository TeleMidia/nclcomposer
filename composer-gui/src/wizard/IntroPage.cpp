/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "../../../include/gui/wizard/IntroPage.h"

IntroPage::IntroPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Introduction");
    labelIntro = new QLabel(this);
    labelIntro->setWordWrap(true);
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->addWidget(labelIntro);
    setLayout(boxLayout);
}
