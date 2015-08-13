package com.intel.rsa.common.mapper;

import org.modelmapper.ModelMapper;
import org.modelmapper.convention.MatchingStrategies;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;

/**
 * Base class for automated mappers.
 * @param <S> Source class
 * @param <T> Target Class
 */
public abstract class BrilliantMapper<S, T> implements Mapper<S, T> {
    private final ModelMapper mapper = new ModelMapper();
    private final Map<Class, Function> providers = new HashMap<>();
    private final Set<Class> skippedTargetMappings = new HashSet<>();

    protected BrilliantMapper() {
        mapper.getConfiguration().setProvider(request -> {
            Function requestedTypeProvider = providers.get(request.getRequestedType());
            return requestedTypeProvider == null ? null : requestedTypeProvider.apply(request.getSource());
        });
        mapper.getConfiguration().setPropertyCondition(context -> !skippedTargetMappings.contains(context.getDestinationType()));
    }

    @Override
    public void map(S source, T target) {
        mapper.map(source, target);
        performNotAutomatedMapping(source, target);
    }

    /**
     * Performs mapping which cannot be automated for some reasons. Try to avoid overriding this method.
     * Provide better automation mechanisms instead.
     */
    protected void performNotAutomatedMapping(S source, T target) {
    }

    protected <X, Y> void registerProvider(Class<Y> targetClass, Function<X, Y> targetClassProvider) {
        providers.put(targetClass, targetClassProvider);
    }

    protected void skipTargetMapping(Class targetClass) {
        skippedTargetMappings.add(targetClass);
    }

    protected void useLooseMatchingStrategy() {
        mapper.getConfiguration().setMatchingStrategy(MatchingStrategies.LOOSE);
    }
}
