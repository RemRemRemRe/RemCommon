// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemTagEventSourceTestTypes.h"

namespace Rem::Common::Private
{
/** the test module's single registry: this definition is its one home */
REMCOMMONTEST_API TFeatureRegistry<FRemTestCustomHandleFamily> FRemTestCustomHandleFamily::Instance;

// The test module owns this family, so it emits the family's instantiations here - a family in any other
// module has the same two lines with that module's export macro.
template class REMCOMMONTEST_API Rem::TFeatureRegistry<FRemTestCustomHandleFamily>;
template class REMCOMMONTEST_API Rem::TFeatureRegistration<FRemTestCustomHandleFamily>;
}
